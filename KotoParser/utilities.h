#include <string>
#include <vector>
#include <map>

using std::wstring;
using std::vector;
using std::map;

#ifndef UTILITIES_H
#define UTILITIES_H

namespace kotoparser
{
	wstring reverse(wstring& input);
	wstring html_encode(wstring input);
	bool is_null_or_whitespace(const wstring& str);

	template <class C, class T>
	int index_of(vector<C> vector, T element)
	{
		return find(vector.begin(), vector.end(), element) - vector.begin();
	}

	int index_of_any(const wstring& input, vector<wchar_t> chars);
	int index_of_any(const wstring& input, vector<wchar_t> chars, int index);
	bool str_starts_with(const wstring& input, const wstring& compare_str);
	bool is_safe_url(wstring url);

	template<class C, class T>
	vector<C> keys(map<C, T> target)
	{
		vector<C> keys;

		for (auto pair : target)
		{
			keys.push_back(pair.first);
		}

		return keys;
	}

	template <typename T>
	void remove(std::vector<T>& vec, size_t pos)
	{
		std::vector<T>::iterator it = vec.begin();
		std::advance(it, pos);
		vec.erase(it);
	}

	template <typename T>
	void remove_range(std::vector<T>& vec, size_t pos, size_t length)
	{
		vec.erase(vec.begin() + pos, vec.begin() + pos + length);
	}

	template <typename T>
	vector<T> sub_vector(vector<T>& target, int first, int length)
	{
		vector<T> new_vector(target.begin() + first, target.begin() + first + length);

		return new_vector;
	}

	template<class C, class T>
	bool contains(vector<C> vec, T val)
	{
		for (auto item : vec)
		{
			if (item == val)
			{
				return true;
			}
		}

		return false;
	}
}

#endif