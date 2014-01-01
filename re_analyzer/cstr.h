#pragma once
#include <string>
#include <iterator>
#include "extvector.h"
#include "encoding.h"

namespace gfutilities{ namespace text
{
	using namespace data_structures;

template<typename string_type>
class cstr : public std::basic_string<string_type>
{
public:
	typedef	std::basic_string<string_type>	base;

public:
	static cstr format(const wchar_t* fmt, ...);
	static cstr format(const char* fmt, ...);

public:
	cstr(const string_type *_Ptr) : base(_Ptr) {}
	cstr(string_type *_Ptr) : base(_Ptr) {}
	cstr() : base() {}

	explicit cstr(const _Alloc& _Al) : base(_Al) {}

	cstr(const _Myt& _Right) : base(_Right) {}

	cstr(const _Myt& _Right, size_type _Roff,
		size_type _Count = npos) : base(_Right, _Roff, _Count) {}

	cstr(const _Myt& _Right, size_type _Roff, size_type _Count,
		const _Alloc& _Al) : base(_Right, _Roff, _Count, _Al) {}

	cstr(const string_type *_Ptr, size_type _Count) 
		: base(_Ptr, _Count) {}
	
	cstr(const string_type *_Ptr, size_type _Count, const _Alloc& _Al) : base(_Ptr, _Count, _Al) {}


	cstr(const string_type *_Ptr, const _Alloc& _Al) : base(_Ptr, _Al) {}

	cstr(size_type _Count, string_type _Ch) : base(_Count, _Ch) {}
	cstr(string_type _Ch, size_type _Count = 1) : base(_Count, _Ch) {}

	cstr(size_type _Count, string_type _Ch, const _Alloc& _Al) : base(_Count, _Ch, _Al) {}

	template<class _It>
	cstr(_It _First, _It _Last) : base(_First, _Last) {}

	template<class _It>
	cstr(_It _First, _It _Last, const _Alloc& _Al) : base(_First, _Last, _Al) {}

	virtual ~cstr(void){}

	operator string_type* () const {
		return (string_type*)this->c_str();
	}

	operator const string_type* () const {
		return (const string_type*)this->c_str();
	}

	friend std::wistream& operator >> ( std::wistream& is, cstr& val)
	{
		is.seekg(0, std::ios::end);
		int size = is.tellg();
		is.seekg(0, std::ios::beg);

		if(size <= 0){
			return is;
		}

		array<string_type> buf(size);		

		is.read(buf, size);
		val.assign(buf, size);

		return is;
	}

	cstr& lower();
	cstr& upper();
	cstr lower_copy() const ;
	cstr upper_copy() const ;
	cstr right(size_type idx) const ;
	cstr left(size_type idx) const ;
	cstr trim() const ;

	extvector<cstr> split(const cstr& delimeter, bool remove_empty_cells = true) const ;
	extvector<cstr> split(const string_type delimeter, bool remove_empty_cells = true) const ;

	// returns a vector with the texts inside the brackets.
	// Notice: function perform a 1 level of nesting in case open and close are different.
	// example: this is [a line [of text]] returns "a line [of text]".
	// to create nesting, call the function recursively.
	extvector<cstr> split_brackets(const cstr& open_bracket, const cstr& close_bracket) const ;

	void replace_all(const cstr& from, const cstr& to);
	void replace(size_type idx, const cstr& from, const cstr& to);

	bool contains(const cstr& other) const;
	bool contains_no_case(const cstr& other) const;
	int	 count(string_type c) const;

	bool starts_with(const cstr& oher) const;
	bool ends_with(const cstr& other) const;

	cstr& xml_encode(void);
	cstr& xml_decode(void);

	bool compare_no_case(const cstr& other) const ;

	void trace(void);

private:
	void trace(const wchar_t* txt);
	void trace(const char* txt);

	bool compare_no_case(const wchar_t* s1, const wchar_t* s2) const ;
	bool compare_no_case(const char* s1, const char* s2) const ;
};

#include "cstr.cpp"

typedef cstr<wchar_t>	wstr;
typedef cstr<char>		astr;
}}