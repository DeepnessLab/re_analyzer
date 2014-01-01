//---------------------------------------------------------------------------
template<typename string_type>
inline cstr<string_type>& cstr<string_type>::lower(void)
{
	transform(begin(), end(), begin(), tolower );
	return *this;
}
//---------------------------------------------------------------------------
template<typename string_type>
inline cstr<string_type>& cstr<string_type>::upper(void)
{
	transform(begin(), end(), begin(), toupper );
	return *this;
}
//---------------------------------------------------------------------------
template<typename string_type>
inline cstr<string_type>cstr<string_type>::upper_copy(void) const 
{
	cstr<string_type> tmp = *this;
	transform(tmp.begin(), tmp.end(), tmp.begin(), toupper );
	return tmp;
}
//---------------------------------------------------------------------------
template<typename string_type>
inline cstr<string_type> cstr<string_type>::lower_copy(void) const
{
	cstr<string_type> tmp = *this;
	transform(tmp.begin(), tmp.end(), tmp.begin(), tolower );
	return tmp;
}
//---------------------------------------------------------------------------
template<typename string_type>
extvector<cstr<string_type>> cstr<string_type>::split( const cstr<string_type>& delimeter, bool remove_empty_cells /*= true*/ ) const 
{
	int length = (int)this->length();
	int start = 0;
	int end = 0;
	int lastDelimeter = 0;
	extvector<cstr<string_type>> vWords;

	while((end = (int)this->find(delimeter, start)) != -1)
	{
		cstr<string_type> temp = this->substr(start, end - start);
		if(!(remove_empty_cells && temp.empty())){
			vWords.push_back(temp);
		}

		if(end < length)
			start = end + delimeter.length();
		else
			break; // end of word - finish

		lastDelimeter = end;
	}

	if(lastDelimeter > 0 && lastDelimeter != length-1)
	{
		cstr<string_type> temp = this->substr(lastDelimeter+delimeter.length(), length - lastDelimeter);

		if(!(remove_empty_cells && temp.empty())){
			vWords.push_back(temp);
		}
	}
	else if(lastDelimeter == 0)
	{
		cstr<string_type> temp = this->substr(lastDelimeter, length);

		if(!(remove_empty_cells && (temp.empty() || temp == delimeter))){
			vWords.push_back(temp);
		}
	}

	return vWords;
}
//---------------------------------------------------------------------------
template<typename string_type>
extvector<cstr<string_type>> cstr<string_type>::split( const string_type delimeter, bool remove_empty_cells /*= true*/ ) const
{
	return split(cstr(delimeter), remove_empty_cells);
}
//---------------------------------------------------------------------------
template<typename string_type>
extvector<cstr<string_type>> cstr<string_type>::split_brackets( const cstr<string_type>& open_bracket, const cstr<string_type>& close_bracket ) const
{
	
	bool different_brackets = open_bracket != close_bracket;
	extvector<cstr<string_type>> res;
	cstr<string_type>::size_type start = 0;
	cstr<string_type>::size_type end = 0;

	int open_bracket_len = (int)open_bracket.length();
	int close_bracket_len = (int)close_bracket.length();

	while(start != cstr<string_type>::npos)
	{
		// find open
		start = find(open_bracket, start);
		if(start == cstr<string_type>::npos)
			break;

		// find close
		end = find(close_bracket, start+1);
		if(end == cstr<string_type>::npos)
			break;

		if(different_brackets) // make sure there is 1 level of nesting
		{
			// while there is an open bracket inside the open/close brackets
			// look for the close bracket:
			// a case like this: [ab[cd] should return "ab[cd"
			// a case like this: [ab[cd]] should return "ab[cd]"
			cstr<string_type>::size_type check_from = start+open_bracket_len;
			while((check_from = find(open_bracket, check_from)) < end)
			{
				cstr<string_type>::size_type new_end = find(close_bracket, end+close_bracket_len);
				if(new_end != cstr<string_type>::npos){
					end = new_end;
				}

				check_from++;
			}
		}

		// insert string
		res.push_back(substr(start+open_bracket_len, (end-close_bracket_len)-start));

		start = end+1;
	}

	return res;
}
//---------------------------------------------------------------------------
template<typename string_type>
void cstr<string_type>::replace_all( const cstr<string_type>& from, const cstr<string_type>& to )
{
	size_t start = 0;

	start = find(from.c_str(), start);
	while(start != -1)
	{
		replace(start, from, to);

		start += to.length();

		start = find(from.c_str(), start);
	}
}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::starts_with( const cstr<string_type>& other ) const
{
	
	int me_len = length();
	int him_len = other.length();

	// if this string is smaller than other - surly it is false.
	if(me_len < him_len){
		return false;
	}

	if(him_len == 0){
		return true;
	}

	const_iterator me = begin();
	const_iterator him = other.begin();

	while(him != other.end() && *me == *him)
	{
		me++;
		him++;
	}

	return him == other.end();	
}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::ends_with( const cstr<string_type>& other ) const
{

	int me_len = length();
	int him_len = other.length();

	// if this string is smaller than other - surly it is false.
	if(me_len < him_len){
		return false;
	}

	if(him_len == 0){
		return true;
	}

	const_reverse_iterator me = rbegin();
	const_reverse_iterator him = other.rbegin();

	while(him != other.rend() && *me == *him)
	{
		me++;
		him++;
	}

	return him == other.rend();		
}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::contains( const cstr<string_type>& other ) const
{	
	return find(other.c_str()) != cstr<string_type>::npos;
}
//---------------------------------------------------------------------------
template<typename string_type>
cstr<string_type> cstr<string_type>::right( size_type idx ) const 
{
	if( idx > length() ){
		empty_throw1(E_ERR_INDEX_TOO_BIG, idx);
	}

	return cstr(end() - idx, end());
}
//---------------------------------------------------------------------------
template<typename string_type>
cstr<string_type> cstr<string_type>::left( size_type idx ) const 
{
	if( idx > length() ){
		throw std::exception(L"index too big");
	}

	return cstr(begin(), begin() + idx);
}
//---------------------------------------------------------------------------
template<typename string_type>
void cstr<string_type>::replace( size_type idx, const cstr<string_type>& from, const cstr<string_type>& to )
{
	erase(idx, from.length());
	insert(idx, to);
}
//---------------------------------------------------------------------------
template<typename string_type>
cstr<string_type> cstr<string_type>::trim() const
{
	cstr<string_type> copy = *this;

	if( copy.length() > 0 )
	{
		for(;;){
			if( _istspace(*copy.begin()) ){
				copy.erase(0, 1);
			}
			else{
				break;
			}
			if( copy.length() == 0 ){
				break;
			}
		}
	}

	if( copy.length() > 0 )
	{
		for(;;){
			if( _istspace(*(copy.end() - 1)) ){
				copy.erase(copy.length() - 1, 1);
			}
			else{
				break;
			}
			if( copy.length() == 0 ){
				break;
			}
		}
	}

	return copy;
}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::compare_no_case( const cstr<string_type>& other ) const 
{
	return compare_no_case( c_str(), other.c_str() );
}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::compare_no_case( const wchar_t* s1, const wchar_t* s2 ) const 
{
	return _wcsicmp( s1, s2) == 0;
}
//-------------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::compare_no_case( const char* s1, const char* s2 ) const 
{
	return stricmp( s1, s2 ) == 0;
}
//-------------------------------------------------------------------------------
template<typename string_type>
cstr<string_type>& cstr<string_type>::xml_encode( void )
{
	replace_all(_T("\""),	_T("&quot;"));
	replace_all(_T("&"),	_T("&amp;"));
	replace_all(_T("'"),	_T("&apos;"));
	replace_all(_T("<"),	_T("&lt;"));
	replace_all(_T(">"),	_T("&gt;"));

	return *this;
}
//---------------------------------------------------------------------------
template<typename string_type>
cstr<string_type>& cstr<string_type>::xml_decode()
{
	replace_all(_T("&quot;"),	_T("\""));
	replace_all(_T("&amp;"),	_T("&"));
	replace_all(_T("&apos;"),	_T("'"));
	replace_all(_T("&lt;"),		_T("<"));
	replace_all(_T("&gt;"),		_T(">"));

	return *this;
}
//---------------------------------------------------------------------------
template<typename string_type>
int cstr<string_type>::count( string_type c ) const
{
	
	return std::count(begin(), end(), c);

}
//---------------------------------------------------------------------------
template<typename string_type>
bool cstr<string_type>::contains_no_case( const cstr<string_type>& other ) const
{
	
	return lower_copy().contains(other.lower_copy());
}
//---------------------------------------------------------------------------
template<typename string_type>
cstr<string_type> cstr<string_type>::format( const wchar_t* fmt, ... )
{
	cstr<string_type> res;

	va_list args;
	va_start(args, fmt);

	std::vector<string_type> resultstring(_vscwprintf(fmt, args) + 1);
	vswprintf_s(&resultstring[0], resultstring.size(), fmt, args);

	va_end(args);

	std::copy(resultstring.begin(), resultstring.end()-1,  std::inserter(res, res.begin()));

	return res;
}
//-------------------------------------------------------------------------------
template<typename string_type>
cstr<string_type> cstr<string_type>::format( const char* fmt, ... )
{
	cstr<string_type> res;

	va_list args;
	va_start(args, fmt);

	std::vector<string_type> resultstring(_vscprintf(fmt, args) + 1);
	vsprintf_s(&resultstring[0], resultstring.size(), fmt, args);

	va_end(args);

	std::copy(resultstring.begin(), resultstring.end()-1,  std::inserter(res, res.begin()));

	return res;
}
//-------------------------------------------------------------------------------
template<typename string_type>
void cstr<string_type>::trace( void )
{
	trace(c_str());
}
//-------------------------------------------------------------------------------
template<typename string_type>
void cstr<string_type>::trace( const wchar_t* txt )
{
	::OutputDebugStringW(txt);
}
//-------------------------------------------------------------------------------
template<typename string_type>
void cstr<string_type>::trace( const char* txt )
{
	::OutputDebugStringA(txt);
}
//-------------------------------------------------------------------------------