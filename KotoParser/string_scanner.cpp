#include "string_scanner.h"
#include <string>
#include <algorithm>
#include <vector>
#include "utilities.h"

using std::wstring;

namespace kotoparser
{
	wchar_t StringScanner::current()
	{
		if (position < start || position >= end)
		{
			return '\0';
		}
		else
		{
			return input[position];
		}
	}
	
	// TODO: ±ú²ýÇÏ°Ô
	bool StringScanner::char_is_first_of_line()
	{
		wstring reversed = kotoparser::reverse(input);
		
		int index = input.length() - position - 1;
		std::wstring::iterator new_line_it = std::find_if(reversed.begin() + index, reversed.end(), [](wchar_t c) -> bool
		{
			return c == '\n' || c == '\r';
		});
		int new_line_index = std::distance(reversed.begin(), new_line_it);

		return is_null_or_whitespace(reversed.substr(index + 1, new_line_index - index));
	}

	bool StringScanner::skipped_line_end()
	{
		return is_line_end(char_at_offset(-1)) && !is_line_end(current());
	}

	bool StringScanner::at_file_start()
	{
		return position == start;
	}

	bool StringScanner::at_file_end()
	{
		return position >= end;
	}

	bool StringScanner::at_line_end()
	{
		return is_line_end(current());
	}

	StringScanner::StringScanner(wstring input)
	{
		reset(input);
	}

	StringScanner::StringScanner(wstring input, int position, int length)
	{
		reset(input, position, length);
	}

	void StringScanner::reset(wstring input)
	{
		reset(input, 0, input.length());
	}

	void StringScanner::reset(wstring input, int position, int length)
	{
		if (position < 0)
		{
			position = 0;
		}

		if ((unsigned)position > input.length())
		{
			position = input.length();
		}

		if (length < 0)
		{
			length = 0;
		}

		this->input = input;
		this->start = position;
		this->position = position;
		this->end = position + length;
		this->mark_pos = position;

		if ((unsigned)end > input.length())
		{
			end = input.length();
		}
	}

	wchar_t StringScanner::char_at_offset(int offset)
	{
		int index = position + offset;

		if (index < start || index >= end)
		{
			return '\0';
		}

		return input[index];
	}

	void StringScanner::skip_forward(int characters)
	{
		position += characters;
	}

	bool StringScanner::skip_whitespace()
	{
		if (!isspace(current()))
		{
			return false;
		}

		do
		{
			skip_forward(1);
		}
		while (isspace(current()));

		return true;
	}

	bool StringScanner::skip_linespace()
	{
		if (!is_linespace(current()))
		{
			return false;
		}

		do
		{
			skip_forward(1);
		}
		while (is_linespace(current()));

		return true;
	}

	void StringScanner::skip_to_line_end()
	{
		while (position < end)
		{
			wchar_t c = input[position];

			if (c == '\r' || c == '\n')
			{
				break;
			}

			++position;
		}
	}

	void StringScanner::skip_line_end()
	{
		if (position < end)
		{
			wchar_t c = input[position];

			if (c == '\r')
			{
				// ÀÏ´Ü ÇÑ ±ÛÀÚ °Ç³Ê¶Ù°í
				++position;

				if (input[position] == '\n')
				{
					++position;
				}
			}
			else if (c == '\n')
			{
				++position;
			}
		}
	}

	void StringScanner::skip_to_next_line()
	{
		skip_to_line_end();
		skip_line_end();
	}

	void StringScanner::skip_to_file_end()
	{
		position = end;
	}

	bool StringScanner::does_match(wchar_t c)
	{
		return current() == c;
	}

	bool StringScanner::does_match_any(vector<wchar_t> chars)
	{
		for (wchar_t c : chars)
		{
			if (does_match(c))
			{
				return true;
			}
		}
		
		return false;
	}

	bool StringScanner::does_match(wstring str)
	{
		for (int i = 0; (unsigned)i < str.length(); ++i)
		{
			if (str[i] != char_at_offset(i))
			{
				return false;
			}
		}

		return true;
	}

	bool StringScanner::find(wchar_t c)
	{
		if (position > end)
		{
			return false;
		}

		wstring::size_type index = input.find(c, position);
		if (index == wstring::npos || index >= (unsigned)end)
		{
			return false;
		}

		position = index;
		return true;
	}

	bool StringScanner::find_any(vector<wchar_t> chars)
	{
		int index = index_of_any(input, chars, position);

		if (index >= end)
		{
			return false;
		}

		position = index;
		return true;
	}

	bool StringScanner::find(wstring str)
	{
		wstring::size_type index = input.find(str, position);
		if (index == wstring::npos || index >= (unsigned)end)
		{
			return false;
		}

		position = index;
		return true;
	}

	wstring StringScanner::substring(int start)
	{
		return input.substr(start, end - start);
	}

	wstring StringScanner::substring(int start, int length)
	{
		if (start + length > end)
		{
			length = end - start;
		}

		return input.substr(start, length);
	}

	void StringScanner::mark()
	{
		mark_pos = position;
	}

	wstring StringScanner::extract()
	{
		if (mark_pos >= position)
		{
			return L"";
		}

		return input.substr(mark_pos, position - mark_pos);
	}

	wstring StringScanner::extract_word_block()
	{
		mark();

		if (iswalpha(input[position]) || input[position] == '_')
		{
			do
			{
				skip_forward(1);
			}
			while ((unsigned)position < input.length() && (iswalnum(input[position]) || input[position] == '_'));
		}

		return extract();
	}

	bool is_linespace(wchar_t c)
	{
		return c == ' ' || c == '\t';
	}

	bool is_line_end(wchar_t c)
	{
		return c == '\r' || c == '\n' || c == '\0';
	}
}