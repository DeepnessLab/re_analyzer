// re_analyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ire_analyzer.h"
#include <boost/program_options.hpp>
#include <boost/exception/exception.hpp>
#include <atlfile.h>
#include "cstr.h"
#include "file.h"
#include "rule.h"
#include "error_code.h"

#define print_usage()	std::cout << desc << std::endl; return 1

namespace po = boost::program_options;
using namespace gfutilities;
using namespace gfutilities::io;
using namespace gfutilities::text;
using namespace gfutilities::error;

Ire_analyzer* load_analyzer( const std::string& library_to_load );
void print_regex_size( Ire_analyzer* analyzer, const std::string& pattern );
void handle_pattern( Ire_analyzer* analyzer, const std::string& pattern );
void handle_file( Ire_analyzer* analyzer, const std::string& file, bool is_concatenate );
void handle_snort( Ire_analyzer* analyzer, const std::string& file, bool is_concatenate );

//-------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE h = NULL;
	po::options_description desc("Regular Expressions Analyzer Options");

	try
	{
		desc.add_options()
			("help", "this help screen")
			("lib,l", po::value<std::string>()->required(), "regex library to analyze")
			("pattern,p", po::value<std::string>(),"input regular expression pattern")
			("file,f", po::value<std::string>(),"input regular expressions file (each line (CRLF) holds a pattern)")
			("snort,s", po::value<std::string>(),"input snort rules file. # is a comment")
			("concatenate,c", po::value<std::string>()->implicit_value(""), "concatenate all expressions using OR");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		// print usage
		if (vm.count("help"))
		{
			print_usage();
		}

		// verify exactly one input has been specified
		if (vm.count("pattern") + vm.count("file") + vm.count("snort") != 1)
		{
			print_usage();
		}

		// load analyzer
		std::string library_to_load = vm["lib"].as<std::string>() + "_analyzer.dll";
		Ire_analyzer* analyzer = load_analyzer(library_to_load);
		if(!analyzer)
		{
			throw error_code(101, astr::format("Failed to load analyzer in the DLL %s", library_to_load.c_str()));
		}

		if(vm.count("pattern")) // pattern input
		{
			handle_pattern(analyzer, vm["pattern"].as<std::string>());
		}
		else if(vm.count("file")) // file input
		{
			handle_file(analyzer, vm["file"].as<std::string>(), vm.count("concatenate") != 0);
		}
		else // snort rules file
		{
			handle_snort(analyzer, vm["snort"].as<std::string>(), vm.count("concatenate") != 0);
		}

		std::cout << "Done" << std::endl;
	}
	catch(po::required_option& /*ro_exp*/)
	{
		print_usage();
	}
	catch(error_code& err)
	{
		std::wcout << "Unexpected error has occurred: (" << err.code << ") " << err.description << std::endl;
	}
	catch(std::exception& ex)
	{
		std::cout << "Unexpected error has occurred: " << ex.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "Unexpected error has occurred" << std::endl;
	}

	if(!h)
	{
		FreeLibrary(h);
	}

	return 0;
}
//-------------------------------------------------------------------------------
Ire_analyzer* load_analyzer( const std::string& library_to_load )
{
	HINSTANCE h = ::LoadLibraryA(library_to_load.c_str());
	if(!h)
	{
		error_code last_error = error_code::get_last_error();
		throw error_code(last_error.code, astr::format("Failed to load library: %S", last_error.description.c_str()));
	}

	Ire_analyzer* (*p_get_analyzer_object)(void) = (Ire_analyzer* (*)(void))::GetProcAddress(h, "get_analyzer_object");
	if(!p_get_analyzer_object)
	{
		error_code last_error = error_code::get_last_error();
		throw error_code(last_error.code, astr::format("Failed getting \"get_analyzer_object\" address: %S", last_error.description.c_str()));
	}


	Ire_analyzer* pobject = p_get_analyzer_object();
	if(!pobject)
	{
		throw error_code(101, "Failed getting analyzer object");
	}

	std::cout << "Loading " << pobject->get_library_name().c_str() << std::endl;

	return pobject;
}
//-------------------------------------------------------------------------------
inline void print_regex_size( Ire_analyzer* analyzer, const std::string& pattern )
{
	std::string error;
	int size = analyzer->get_regex_size(pattern, error);
	if(!error.empty()){
		std::cout << "ERROR COMPILING \"" << pattern.c_str() << "\": " << error.c_str() << std::endl;
	}
	else{
		std::cout << "\"" << pattern.c_str() << "\": " << size << std::endl;
	}
}
//-------------------------------------------------------------------------------
void handle_pattern( Ire_analyzer* analyzer, const std::string& pattern )
{
	print_regex_size(analyzer, pattern);
}
//-------------------------------------------------------------------------------
void handle_file( Ire_analyzer* analyzer, const std::string& ascii_filename, bool is_concatenate )
{
	std::wstring filename = encoding::utf8_to_unicode(ascii_filename);

	if(!file::is_exist(filename)){
		throw error_code(101, astr::format("The input file %s cannot be found", ascii_filename.c_str()));
	}
	
	std::cout << "loading " << ascii_filename.c_str() << std::endl;

	// read content
	file f(filename);
	astr all_content;
	f.read(all_content);
	f.close();

	data_structures::extvector<astr> lines = all_content.split("\r\n");
	std::cout << lines.size() << " patterns have been loaded" << std::endl;

	if(is_concatenate)
	{
		std::cout << "concatenating patterns" << std::endl;
		astr concatenated_pattern;

		for(data_structures::extvector<astr>::iterator it = lines.begin() ; it != lines.end() ; it++)
		{
			concatenated_pattern += *it;
			concatenated_pattern += "|";
		}

		if(concatenated_pattern.ends_with("|")){
			concatenated_pattern.erase(concatenated_pattern.length()-1);
		}

		print_regex_size(analyzer, concatenated_pattern);
	}
	else
	{
		for(data_structures::extvector<astr>::iterator it = lines.begin() ; it != lines.end() ; it++)
		{
			print_regex_size(analyzer, *it);
		}
	}
}
//-------------------------------------------------------------------------------
void handle_snort( Ire_analyzer* analyzer, const std::string& ascii_filename, bool is_concatenate )
{
	std::wstring filename = encoding::utf8_to_unicode(ascii_filename);

	if(!file::is_exist(filename)){
		throw error_code(101, astr::format("The input file %s cannot be found", ascii_filename.c_str()));
	}

	std::cout << "loading " << ascii_filename.c_str() << std::endl;

	// read content
	file f(filename);
	astr all_content;
	f.read(all_content);
	f.close();

	data_structures::extvector<astr> lines = all_content.split("\r\n");
	std::cout << lines.size() << " patterns have been loaded" << std::endl;

	if(is_concatenate)
	{
		std::cout << "concatenating patterns" << std::endl;
		astr concatenated_pattern;

		for(data_structures::extvector<astr>::iterator it = lines.begin() ; it != lines.end() ; it++)
		{
			if(!it->starts_with("#"))
			{
				rule r(*it);
				concatenated_pattern += r.regex;
				concatenated_pattern += "|";
			}
		}

		if(concatenated_pattern.ends_with("|")){
			concatenated_pattern.erase(concatenated_pattern.length()-1);
		}

		print_regex_size(analyzer, concatenated_pattern);
	}
	else
	{
		for(data_structures::extvector<astr>::iterator it = lines.begin() ; it != lines.end() ; it++)
		{
			if(!it->starts_with("#"))
			{
				rule r(*it);
				std::cout << "RuleID: " << r.ruleid << "  ";
				print_regex_size(analyzer, r.regex);
			}
		}
	}
}
//-------------------------------------------------------------------------------