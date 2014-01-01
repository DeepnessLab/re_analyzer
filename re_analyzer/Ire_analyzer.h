#pragma once
#include <string>

#define FAILED_CALC_REGEX_SIZE (UINT)~0

struct Ire_analyzer
{
	virtual std::string get_library_name(void) = 0;
	virtual UINT get_regex_size(const std::string& pattern, std::string& out_error) = 0;
};