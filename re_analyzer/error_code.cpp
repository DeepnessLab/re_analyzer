#include "StdAfx.h"
#include <string>
#include "error_code.h"
#include "encoding.h"

namespace gfutilities{ namespace error
{
	using namespace text;
//-------------------------------------------------------------------------------
error_code::error_code( const std::string& desc ):_code(0)
{
	_description = encoding::utf8_to_unicode(desc);
}
//-------------------------------------------------------------------------------
error_code::error_code( DWORD code, const std::string& desc ):_code(code)
{
	_description = encoding::utf8_to_unicode(desc);
}
//-------------------------------------------------------------------------------
error_code error_code::get_last_error( void )
{
	wchar_t lpMsgBuf[4096] = {0};
	DWORD err = ::GetLastError();

	DWORD len = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL,
								err,
								0, // Default language
								lpMsgBuf,
								4095,
								NULL);

	if( len )
	{
		return error_code(err, lpMsgBuf);
	}
	else
	{
		return error_code(err, L"Failed to description of the given error.");
	}
}
//-------------------------------------------------------------------------------
}}