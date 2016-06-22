#include <string>
#include <vector>
using std::wstring;
using std::vector;

#ifndef STRINGSCANNER_H
#define STRINGSCANNER_H

namespace kotoparser
{
	class StringScanner
	{
	public:
		wchar_t current();
		bool char_is_first_of_line();
		bool skipped_line_end();
		bool at_file_start();
		bool at_file_end();
		bool at_line_end();

		StringScanner(wstring input);
		StringScanner(wstring input, int position, int length);

		void reset(wstring input);
		void reset(wstring input, int position, int length);

		wchar_t char_at_offset(int offset);
		void skip_forward(int characters);
		bool skip_whitespace();
		int skip_linespace();
		void skip_to_line_end();
		void skip_line_end();
		void skip_to_next_line();
		void skip_to_file_end();

		bool does_match(wchar_t c);
		bool does_match_any(vector<wchar_t> chars);
		bool does_match(wstring str);

		bool find(wchar_t c);
		bool find_any(vector<wchar_t> chars);
		bool find(wstring str);

		wstring substring(int start);
		wstring substring(int start, int length);

		void mark();
		wstring extract();
		wstring extract_word_block();
	protected:
		wstring input;
		int start;
		int end;
		int position;
		int mark_pos;
	};

	bool is_linespace(wchar_t c);
	bool is_line_end(wchar_t c);
}
#endif