#pragma once
#include <atlfile.h>
#include <string>
#include <vector>
#include "properties.h"

namespace gfutilities{ namespace io
{
//-------------------------------------------------------------------------------
class file : public ATL::CAtlFile
{
public:
	static HRESULT	create(const std::wstring& filename);
	static bool		is_exist(const std::wstring& filename);
	static bool		delete_file(const std::wstring& filename);
	
public:
	file(const std::wstring& filename, bool is_open = true);
	file(const file& other);
	virtual ~file(void){}

	property_get_implemented(const std::wstring&, filename);
	property_get(ULONGLONG,		size);
	property_get(std::wstring,	extension);
	property_get(std::wstring,	path);
	property_get(std::wstring,	name);
	
	void operator = (const file& other){ *this = other; }
	
	HRESULT		open(DWORD access = GENERIC_ALL, DWORD share = NULL, DWORD creation = OPEN_EXISTING);
	void		close(void);
	
	HRESULT		read(std::string& out_bytes);
	HRESULT		read(std::wstring& out_text);
	HRESULT		write(const char* bytes, ULONGLONG size, DWORD* out_written = NULL);
	HRESULT		write(const std::string& text, DWORD* out_written = NULL);
	HRESULT		write(const std::wstring& text, DWORD* out_written = NULL);
	HRESULT		append(const char* bytes, ULONGLONG size, DWORD* out_written = NULL);
	HRESULT		append(const std::string& text, DWORD* out_written = NULL);
	HRESULT		append(const std::wstring& text, DWORD* out_written = NULL);

	HRESULT		flush(void);
	
private:
	file(void){}

	std::wstring _filename;
};
//-------------------------------------------------------------------------------
}}