#include "utilities.h"
#include <string>
#include <algorithm>
#include <vector>

using std::wstring;

namespace kotoparser
{
	wstring reverse(wstring& input)
	{
		wstring result{ input };
		std::reverse(result.begin(), result.end());

		return result;
	}

	wstring html_encode(wstring& data)
	{
		wstring buffer;
		buffer.reserve(data.size());
		for (size_t pos = 0; pos != data.size(); ++pos)
		{
			switch (data[pos])
			{
				case '&':  buffer.append(L"&amp;");       break;
				case '\"': buffer.append(L"&quot;");      break;
				case '\'': buffer.append(L"&apos;");      break;
				case '<':  buffer.append(L"&lt;");        break;
				case '>':  buffer.append(L"&gt;");        break;
				default:   buffer.append(&data[pos], 1); break;
			}
		}
		return buffer;
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

	bool str_starts_with(const wstring& input, const wstring& compare_str)
	{
		return input.compare(0, compare_str.length(), compare_str) == 0;
	}
}