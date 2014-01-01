#pragma once
#include "properties.h"
#include "encoding.h"

namespace gfutilities{ namespace error
{
//-------------------------------------------------------------------------------
class error_code
{
public:
	static error_code get_last_error(void);

public:
	error_code(void):_code(0){}
	error_code(DWORD code):_code(code){}
	error_code(const std::wstring& desc):_code(0), _description(desc){}
	error_code(DWORD code, const std::wstring& desc):_code(code), _description(desc){}
	error_code(const std::string& desc);
	error_code(DWORD code, const std::string& desc);
	virtual ~error_code(void){}

	property_get_set_implemented(DWORD, code);
	property_get_set_implemented(std::wstring, description);


protected:
	DWORD			_code;
	std::wstring	_description;
};
//-------------------------------------------------------------------------------
}}