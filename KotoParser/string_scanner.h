#include <string>
#include <vector>
using std::string;
using std::vector;

#ifndef STRINGSCANNER_H
#define STRINGSCANNER_H

namespace kotoparser
{
	class StringScanner
	{
	public:
		char current();
		bool char_is_first_of_line();
		bool skipped_line_end();
		bool at_file_start();
		bool at_file_end();
		bool at_line_end();

		StringScanner(string input);
		StringScanner(string input, int position, int length);

		void reset(string input);
		void reset(string input, int position, int length);

		char char_at_offset(int offset);
		void skip_forward(int characters);
		bool skip_whitespace();
		int skip_linespace();
		void skip_to_line_end();
		void skip_line_end();
		void skip_to_next_line();
		void skip_to_file_end();

		bool does_match(char c);
		bool does_match_any(vector<char> chars);
		bool does_match(string str);

		bool find(char c);
		bool find_any(vector<char> chars);
		bool find(string str);

		string substring(int start);
		string substring(int start, int length);

		void mark();
		string extract();
		string extract_word_block();
	protected:
		string input;
		int start;
		int end;
		int position;
		int mark_pos;
	};

	bool is_linespace(char c);
	bool is_line_end(char c);
}
#endif