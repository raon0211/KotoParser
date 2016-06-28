#include "utilities.h"
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using std::string;
using std::map;

namespace kotoparser
{
	string reverse(string& input)
	{
		string result{ input };
		std::reverse(result.begin(), result.end());

		return result;
	}

	string html_encode(string data)
	{
		string buffer;
		buffer.reserve(data.size());
		for (size_t pos = 0; pos != data.size(); ++pos)
		{
			switch (data[pos])
			{
				case '&':  buffer.append("&amp;");       break;
				case '\"': buffer.append("&quot;");      break;
				case '\'': buffer.append("&apos;");      break;
				case '<':  buffer.append("&lt;");        break;
				case '>':  buffer.append("&gt;");        break;
				default:   buffer.append(&data[pos], 1); break;
			}
		}
		return buffer;
	}

	bool is_null_or_whitespace(const string& str)
	{
		return std::find_if(
			str.begin(),
			str.end(),
			[](char ch)
			{
				return !iswspace(ch);
			})
			== str.end();
	}

	int index_of_any(const string& input, vector<char> chars)
	{
		return index_of_any(input, chars, 0);
	}

	int index_of_any(const string& input, vector<char> chars, int index)
	{
		std::string::const_iterator it = std::find_if(input.begin() + index, input.end(), [&chars](unsigned char c) -> bool
		{
			return std::find(chars.begin(), chars.end(), c) != chars.end();
		});
		return std::distance(input.begin(), it);
	}

	bool str_starts_with(const string& input, const string& compare_str)
	{
		return input.compare(0, compare_str.length(), compare_str) == 0;
	}

	bool is_safe_url(string url)
	{
		return str_starts_with(url, "http://") ||
			str_starts_with(url, "https://") ||
			str_starts_with(url, "ftp://");
	}
}