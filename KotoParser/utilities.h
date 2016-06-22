#include <string>
#include <vector>

using std::wstring;
using std::vector;

#ifndef UTILITIES_H
#define UTILITIES_H

namespace kotoparser
{
	wstring reverse(wstring& input);
	wstring html_encode(wstring& input);
	bool is_null_or_whitespace(const wstring& str);
	int index_of_any(const wstring& input, vector<wchar_t> chars);
	int index_of_any(const wstring& input, vector<wchar_t> chars, int index);
	bool str_starts_with(const wstring& input, const wstring& compare_str);

	template<class C, class T>
	auto contains(const C& v, const T& x)
		-> decltype(end(v), true)
	{
		return end(v) != std::find(begin(v), end(v), x);
	}
}

#endif