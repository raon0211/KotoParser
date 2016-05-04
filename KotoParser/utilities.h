#include <string>
#include <vector>

using std::wstring;
using std::vector;

#ifndef UTILITIES_H
#define UTILITIES_H

namespace kotoparser
{
	wstring reverse(wstring input);
	bool is_null_or_whitespace(const wstring& str);
	int index_of_any(const wstring& input, vector<wchar_t> chars);
	int index_of_any(const wstring& input, vector<wchar_t> chars, int index);
}

#endif