//---------------------------------------------------------------------------
template<typename T>
std::wstring extvector<T>::to_string(const std::wstring& delimiter) const
{
	int s = size();
	std::wstring result;
	for(int i=0 ; i<s ; i++)
	{
		result += (*this)[i];

		if(i+1 < s){ // if not last item, add delimiter
			result += delimiter;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
template<typename T>
bool extvector<T>::contains( const T& key ) const
{
	return std::find(begin(), end(), key) != end();
}
//---------------------------------------------------------------------------
template<typename T>
bool extvector<T>::remove( const T& key )
{
	iterator it = std::find(begin(), end(), key);

	if(it == end()){
		return false;
	}

	erase(it);
	return true;
}
//---------------------------------------------------------------------------