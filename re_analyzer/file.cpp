#include "StdAfx.h"
#include "file.h"
#include <comdef.h>
#include <io.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <Shlwapi.h>

namespace gfutilities{ namespace io
{
//-------------------------------------------------------------------------------
file::file( const std::wstring& filename, bool is_open /*= false*/ ) : _filename(filename)
{	
	if(is_open){
		open();
	}
}
//-------------------------------------------------------------------------------
file::file( const file& other )
{
	_filename = other._filename;
}
//-------------------------------------------------------------------------------
HRESULT file::create( const std::wstring& filename )
{
	CAtlFile f;
	return f.Create(filename.c_str(), GENERIC_ALL, NULL, CREATE_ALWAYS);
}
//-------------------------------------------------------------------------------
bool file::is_exist( const std::wstring& filename )
{
	WIN32_FIND_DATA FindFileData;

	HANDLE h = ::FindFirstFile(filename.c_str(), &FindFileData);

	if(h != INVALID_HANDLE_VALUE)
	{
		::FindClose(h);
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------
HRESULT file::open( DWORD access /*= GENERIC_ALL*/, DWORD share /*= NULL*/, DWORD creation /*= OPEN_EXISTING*/ )
{
	return Create(_filename.c_str(), access, share, creation);
}
//-------------------------------------------------------------------------------
void file::close( void )
{
	this->Close();
}
//-------------------------------------------------------------------------------
HRESULT file::read( std::string& out_bytes )
{
	ULONGLONG fsize = size;
	out_bytes.resize((UINT)fsize);

	return Read(const_cast<char*>(out_bytes.c_str()), (DWORD)fsize);
}
//-------------------------------------------------------------------------------
HRESULT file::write( const char* bytes, ULONGLONG size, DWORD* out_written /*= NULL*/ )
{
	return Write((LPCVOID)bytes, (DWORD)size, out_written);
}
//-------------------------------------------------------------------------------
HRESULT file::flush( void )
{
	return Flush();
}
//-------------------------------------------------------------------------------
ULONGLONG file::get_size( void ) const
{
	ULONGLONG res;
	GetSize(res);

	return res;
}
//-------------------------------------------------------------------------------
HRESULT file::read( std::wstring& out_text )
{
	std::string content;
	HRESULT hr = read(content);
	if(FAILED(hr)){
		return hr;
	}

	out_text.assign((const wchar_t*)content.c_str(), content.size()/sizeof(wchar_t));
	return hr;
}
//-------------------------------------------------------------------------------
HRESULT file::write( const std::wstring& text, DWORD* out_written /*= NULL*/ )
{
	return write((const char*)text.c_str(), text.length()*sizeof(wchar_t), out_written);
}
//-------------------------------------------------------------------------------
HRESULT file::write( const std::string& text, DWORD* out_written /*= NULL*/ )
{
	return write((const char*)text.c_str(), text.length()*sizeof(char), out_written);
}
//-------------------------------------------------------------------------------
bool file::delete_file( const std::wstring& filename )
{
	return ::DeleteFileW(filename.c_str()) == TRUE;
}
//-------------------------------------------------------------------------------
HRESULT file::append( const char* bytes, ULONGLONG size, DWORD* out_written /*= NULL*/ )
{
	this->Seek(0, FILE_END);
	return write(bytes, size, out_written);
}
//-------------------------------------------------------------------------------
HRESULT file::append( const std::string& text, DWORD* out_written /*= NULL*/ )
{
	return append((const char*)text.c_str(), text.length()*sizeof(char), out_written);
}
//-------------------------------------------------------------------------------
HRESULT file::append( const std::wstring& text, DWORD* out_written /*= NULL*/ )
{
	return append((const char*)text.c_str(), text.length()*sizeof(wchar_t), out_written);
}
//-------------------------------------------------------------------------------
std::wstring file::get_extension(void) const
{
	std::wstring ext(::PathFindExtension(_filename.c_str()));
	transform(ext.begin(), ext.end(), ext.begin(), tolower );
	return ext;
}
//---------------------------------------------------------------------------
std::wstring file::get_path(void) const
{
	if(::PathIsURL(_filename.c_str()))
	{
		return std::wstring(_filename.begin(), _filename.begin() + _filename.rfind( L"/" ) + 1);
	}
	else
	{
		return std::wstring(_filename.begin(), _filename.begin() + _filename.rfind( L"\\" ) + 1);
	}
}
//---------------------------------------------------------------------------
std::wstring file::get_name(void) const
{
	return ::PathFindFileName(_filename.c_str());
}
//-------------------------------------------------------------------------------
}}