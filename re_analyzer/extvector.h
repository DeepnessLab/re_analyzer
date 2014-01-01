#pragma once
#include <vector>

namespace gfutilities{ namespace data_structures
{
template<typename T>
class extvector : public std::vector<T>
{
public:
	typedef vector<T>::iterator iterator;

	extvector(void){}

	std::wstring to_string(const std::wstring& delimiter) const;
	bool contains(const T& key) const;
	bool remove(const T& key);
};

#include "extvector.cpp"
}}