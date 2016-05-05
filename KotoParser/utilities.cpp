#include "utilities.h"
#include <string>
#include <algorithm>
#include <vector>

using std::wstring;

namespace kotoparser
{
	wstring reverse(wstring input)
	{
		wstring result{ input };
		std::reverse(result.begin(), result.end());

		return result;
	}

	bool is_null_or_whitespace(const wstring& str)
	{
		return std::find_if(
			str.begin(),
			str.end(),
			[](wchar_t ch)
			{
				return !iswspace(ch);
			})
			== str.end();
	}

	int index_of_any(const wstring& input, vector<wchar_t> chars)
	{
		return index_of_any(input, chars, 0);
	}

	int index_of_any(const wstring& input, vector<wchar_t> chars, int index)
	{
		std::wstring::const_iterator it = std::find_if(input.begin() + index, input.end(), [&chars](unsigned char c) -> bool
		{
			return std::find(chars.begin(), chars.end(), c) != chars.end();
		});
		return std::distance(input.begin(), it);
	}
}