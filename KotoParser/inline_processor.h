#ifndef INLINE_PROCESSOR_H
#define INLINE_PROCESSOR_H

#include <array>
#include <string>
#include <memory>
#include "string_scanner.h"
#include "token.h"
#include "tokens.h"

using std::wstring;
using std::shared_ptr;
using std::array;

namespace kotoparser
{
	class InlineProcessor : public StringScanner
	{
	public:
		InlineProcessor(wstring input) : StringScanner(input)
		{
		}

		InlineProcessor(wstring input, int start, int length) : StringScanner(input, start, length)
		{
		}

		wstring transform();
		vector<shared_ptr<Token>> process();
		
	private:
		vector<shared_ptr<Token>> parse();
		shared_ptr<Token> make_emphasis_token();
		bool is_emphasis_char(wchar_t c);
		shared_ptr<Token> make_code_token();
		shared_ptr<Token> make_plain_token();
		bool make_html_token(shared_ptr<HtmlToken> token);
		shared_ptr<HtmlTag> parse_tag();
		shared_ptr<Token> make_link_or_image_token();

		vector<shared_ptr<Token>> integrate(vector<shared_ptr<Token>>& tokens);
		vector<shared_ptr<Token>> InlineProcessor::create_emphasis_hierarchy(vector<shared_ptr<Token>> tokens, map<int, vector<array<shared_ptr<EmphasisParseToken>, 2>>> depth_tokens_dic, int depth, int index);
	};
}

#endif