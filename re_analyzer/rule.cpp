#include "StdAfx.h"
#include "rule.h"
#include <sstream>


//-------------------------------------------------------------------------------
rule::~rule( void )
{
}
//-------------------------------------------------------------------------------
void rule::parse_rule()
{
	if(!_raw.starts_with("alert")){
		throw std::exception("not a rule");
	}

	extvector<astr> parts = _raw.split("; ");

	// loop through parts.
	for(int i=0 ; i<(int)parts.size() ; i++)
	{
		if(parts[i].starts_with("content:\"")) // if content
		{
			parse_content(parts[i]);
		}
		else if(parts[i].starts_with("pcre:\""))
		{
			_is_include_pcre = true;
			parse_pcre(parts[i]);
		}
		else if(parts[i].starts_with("sid:"))
		{
			astr strruleid = parts[i];
			strruleid.replace_all("sid:", "");
			std::istringstream((std::string)strruleid) >> _ruleid;
		}
		else if(parts[i].starts_with("nocase"))
		{
			_nocase = true;
		}
	}
}
//-------------------------------------------------------------------------------
astr rule::to_string()
{
	astr res = astr::format("ruleid: %d\r\n", ruleid);
	for(int i=0 ; i<(int)_contents.size() ; i++)
	{
		res += astr::format("content %d: \"%s\"", i, _contents[i]._text.c_str());
		if(i+1<(int)_contents.size()){
			res += "\r\n";
		}
	}

	if(_is_include_pcre){
		res += astr::format("\r\npcre: \"%s\"", _regex.c_str());
	}

	return res;
}
//-------------------------------------------------------------------------------
void rule::parse_content( astr content_text )
{
	content_text.erase(0, 9); // delete 'content:"' text
	content_text.erase(content_text.length()-1); // delete last character (which is '"');
	
	extvector<astr> strbyteslist = content_text.split_brackets("|", "|");
	for(int i=0 ; i<(int)strbyteslist.size() ; i++)
	{
		astr strbytes = strbyteslist[i];
		strbytes.replace_all(" ", "");
		astr res;
		for(int j=0 ; j<(int)strbytes.size() ; j+=2)
		{
			astr temp = strbytes.at(j);
			temp += strbytes.at(j+1);
			int b;
			std::stringstream iss;
			iss << std::hex << (std::string)temp;
			iss >> b;
			res += (char)b;
		}

		content_text.replace_all(strbyteslist[i], res); // replace string-bytes in content attribute.
	}

	// remove all '|'
	content_text.replace_all("|", "");

	_contents.push_back(content(this, _contents.size()+1, content_text));
}
//-------------------------------------------------------------------------------
bool rule::get_matched( void ) const
{
	for(int i=0 ; i<(int)_contents.size() ; i++)
	{
		if(!_contents[i]._matched){
			return false;
		}
	}

	return true;
}
//-------------------------------------------------------------------------------
void rule::clear_matches()
{
	for(int i=0 ; i<(int)_contents.size() ; i++){
		_contents[i]._matched = false;
	}
}
//-------------------------------------------------------------------------------
void rule::operator=( const rule& other )
{
	_contents = other.contents;
	_is_include_pcre = other._is_include_pcre;
	_raw = other._raw;
	_ruleid = other._ruleid;
	_regex = other._regex;

	for(int i=0 ; i<(int)_contents.size() ; i++){
		_contents[i]._parent = (rule*)this;
	}
}
//-------------------------------------------------------------------------------
void rule::parse_pcre( astr strpcre )
{
	strpcre.erase(0, 7); // delete 'pcre:"/' text
	strpcre.erase(strpcre.length()-1); // delete last character (which is '"');

	astr options = strpcre.substr(strpcre.rfind('/'));
	strpcre.erase(strpcre.length()-options.length());

	_regex = strpcre;

}
//-------------------------------------------------------------------------------