#include "string_scanner.h"
#include <string>
#include <algorithm>
#include <vector>
#include "utilities.h"

using std::string;

namespace kotoparser
{
	char StringScanner::current()
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
	
	bool StringScanner::char_is_first_of_line()
	{
		int start = position;

		do
		{
			skip_forward(-1);
		}
		while (is_linespace(current()));

		bool is_first_of_line = is_line_end(current());
		position = start;

		return is_first_of_line;
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

	StringScanner::StringScanner(string input)
	{
		reset(input);
	}

	StringScanner::StringScanner(string input, int position, int length)
	{
		reset(input, position, length);
	}

	void StringScanner::reset(string input)
	{
		reset(input, 0, input.length());
	}

	void StringScanner::reset(string input, int position, int length)
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

	char StringScanner::char_at_offset(int offset)
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

	int StringScanner::skip_linespace()
	{
		int indent = 0;

		while (is_linespace(current()))
		{
			skip_forward(1);
			indent++;
		}

		return indent;
	}

	void StringScanner::skip_to_line_end()
	{
		while (position < end)
		{
			char c = input[position];

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
			char c = input[position];

			if (c == '\r')
			{
				// 일단 한 글자 건너뛰고
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

	bool StringScanner::does_match(char c)
	{
		return current() == c;
	}

	bool StringScanner::does_match_any(vector<char> chars)
	{
		for (char c : chars)
		{
			if (does_match(c))
			{
				return true;
			}
		}
		
		return false;
	}

	bool StringScanner::does_match(string str)
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

	bool StringScanner::find(char c)
	{
		if (position > end)
		{
			return false;
		}

		string::size_type index = input.find(c, position);
		if (index == string::npos || index >= (unsigned)end)
		{
			return false;
		}

		position = index;
		return true;
	}

	bool StringScanner::find_any(vector<char> chars)
	{
		int index = index_of_any(input, chars, position);

		if (index >= end)
		{
			return false;
		}

		position = index;
		return true;
	}

	bool StringScanner::find(string str)
	{
		string::size_type index = input.find(str, position);
		if (index == string::npos || index >= (unsigned)end)
		{
			return false;
		}

		position = index;
		return true;
	}

	string StringScanner::substring(int start)
	{
		return input.substr(start, end - start);
	}

	string StringScanner::substring(int start, int length)
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

	string StringScanner::extract()
	{
		if (mark_pos >= position)
		{
			return "";
		}

		return input.substr(mark_pos, position - mark_pos);
	}

	string StringScanner::extract_word_block()
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

	bool is_linespace(char c)
	{
		return c == ' ' || c == '\t';
	}

	bool is_line_end(char c)
	{
		return c == '\r' || c == '\n' || c == '\0';
	}
}