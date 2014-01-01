#include "stdafx.h"
#include "re2_analyzer.h"
#include <re2\re2.h>

#ifdef _DEBUG
	#pragma comment(lib, ".\\..\\debug\\re2.lib")
#else
	#pragma comment(lib, ".\\..\\release\\re2.lib")
#endif

using namespace re2;

// This is an example of an exported function.
//-------------------------------------------------------------------------------
RE2_ANALYZER_API Ire_analyzer* get_analyzer_object(void)
{
	return new re2_analyzer();
}
//-------------------------------------------------------------------------------
std::string re2_analyzer::get_library_name( void )
{
	return RE2::version();
}
//-------------------------------------------------------------------------------
UINT re2_analyzer::get_regex_size( const std::string& pattern, std::string& out_error )
{
	RE2 re(pattern, RE2::Quiet);
	if(!re.ok())
	{
		out_error = re.error();
		return FAILED_CALC_REGEX_SIZE;
	}

	return re.ProgramSize();
}
//-------------------------------------------------------------------------------