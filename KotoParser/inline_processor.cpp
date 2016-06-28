#include "inline_processor.h"

#include <array>
#include <map>
#include "utilities.h"

using std::map;
using std::array;

namespace kotoparser
{
	string InlineProcessor::transform()
	{
		vector<shared_ptr<Token>> tokens = process();

		string result;

		for (shared_ptr<Token> token : tokens)
		{
			result += token->render();
		}

		return result;
	}

	vector<shared_ptr<Token>> InlineProcessor::process()
	{
		vector<shared_ptr<Token>> tokens = parse();
		tokens = integrate(tokens);
		return tokens;
	}

	vector<shared_ptr<Token>> InlineProcessor::parse()
	{
		vector<shared_ptr<Token>> tokens;

		int start = position;

		while (!at_file_end())
		{
			shared_ptr<Token> token = NULL;
			int line_start = position;

			switch (current())
			{
				// **emphasis** __emphasis__
				case '*':
				case '_':
					token = make_emphasis_token();
					break;

				// `code`
				case '`':
					token = make_code_token();
					break;

				// %%nowiki%%
				case '%':
					token = make_plain_token();
					break;

				// inline html
				case '<':
					token = shared_ptr<HtmlToken>(new HtmlToken(input));

					if (!make_html_token(std::dynamic_pointer_cast<HtmlToken>(token)))
					{
						token = NULL;
						position = line_start;
					}
					break;

				case '!':
				case '[':
					token = make_link_or_image_token();

					if (token == NULL)
					{
						position = line_start;
					}
					break;
			}

			if (token != NULL)
			{
				if (line_start > start)
				{
					tokens.push_back(shared_ptr<TextToken>(new TextToken(input, start, line_start - start)));
				}

				tokens.push_back(token);

				start = position;
			}
			else
			{
				skip_forward(1);
			}
		}

		if (position > start)
		{
			tokens.push_back(shared_ptr<TextToken>(new TextToken(input, start, position - start)));
		}

		return tokens;
	}

	shared_ptr<Token> InlineProcessor::make_emphasis_token()
	{
		char c = current();

		int start = position;

		// if * or _ is connected from the start to the end, consider it as a text
		// e.g. Hello Mr. *** <- *** is text
		if (at_file_start() ||
			is_linespace(char_at_offset(-1)))
		{
			while (is_emphasis_char(current()) && !at_file_end())
			{
				skip_forward(1);
			}

			if (at_file_end() ||
				is_linespace(current()))
			{
				return shared_ptr<TextToken>(new TextToken(input, start, position - start));
			}

			// No it isn't, go back parsing
			position = start;
		}

		// Check if it is an opening tag
		while (is_emphasis_char(char_at_offset(-1)) && position > this->start)
		{
			skip_forward(-1);
		}

		bool space_before = at_file_start() || 
							is_linespace(char_at_offset(-1)) ||
							char_at_offset(-1) == '\n' ||
							char_at_offset(-1) == '\0';
		position = start;

		skip_forward(1);

		while (is_emphasis_char(current()) && !at_file_end())
		{
			skip_forward(1);
		}

		bool space_after = at_file_end() ||
						   is_linespace(current()) ||
						   is_line_end(current()) ||
						   current() == '\0';

		position = start;
		skip_forward(1);

		if (space_before)
		{
			return shared_ptr<EmphasisParseToken>(new EmphasisParseToken(input, start, 1, EmphasisType::Opening));
		}

		if (space_after)
		{
			return shared_ptr<EmphasisParseToken>(new EmphasisParseToken(input, start, 1, EmphasisType::Closing));
		}

		return shared_ptr<EmphasisParseToken>(new EmphasisParseToken(input, start, 1, EmphasisType::Internal));
	}

	bool InlineProcessor::is_emphasis_char(char c)
	{
		return c == '*' || c == '_';
	}

	shared_ptr<Token> InlineProcessor::make_code_token()
	{
		int start = position;

		int count = 0;

		while (does_match('`') && !at_file_end())
		{
			skip_forward(1);
			count++;
		}

		if (at_file_end())
		{
			return shared_ptr<TextToken>(new TextToken(input, start, position - start));
		}

		int content_start = position;

		if (!find(substring(start, count)))
		{
			position = start;
			skip_forward(1);

			return shared_ptr<TextToken>(new TextToken(input, start, 1));
		}

		int content_end = position;

		skip_forward(count);

		return shared_ptr<CodeToken>(new CodeToken(input, content_start, content_end - content_start));
	}

	shared_ptr<Token> InlineProcessor::make_plain_token()
	{
		int start = position;

		int count = 0;

		while (does_match('%') && !at_file_end())
		{
			skip_forward(1);
			count++;
		}

		if (at_file_end())
		{
			return shared_ptr<TextToken>(new TextToken(input, start, position - start));
		}

		// at least two %s, like %%nowiki%%
		if (count < 2)
		{
			position = start;
			skip_forward(1);

			return shared_ptr<TextToken>(new TextToken(input, start, 1));
		}

		int content_start = position;

		if (!find(substring(start, count)))
		{
			position = start;
			skip_forward(1);

			return shared_ptr<TextToken>(new TextToken(input, start, 1));
		}

		int content_end = position;

		skip_forward(count);

		return shared_ptr<TextToken>(new TextToken(input, content_start, content_end - content_start));
	}

	bool InlineProcessor::make_html_token(shared_ptr<HtmlToken> token)
	{
		int start = position;

		shared_ptr<HtmlTag> opening_tag = parse_tag();

		if (opening_tag == NULL)
		{
			return false;
		}

		int content_start = position;

		// Closing tag came first, wrong
		if (opening_tag->closing())
		{
			return false;
		}

		// Is the opening tag safe?
		bool safe = opening_tag->safe();

		vector<HtmlTagType> types = opening_tag->types();

		if (contains(types, HtmlTagType::NoClosing) ||
			opening_tag->self_closed())
		{
			skip_linespace();
			skip_line_end();

			token->tag(opening_tag)
				.end(position)
				.type(safe ? TokenType::Html : TokenType::Text);

			return true;
		}

		vector<shared_ptr<Token>> child_tokens;

		int depth = 1;

		while (!at_file_end())
		{
			// Failed to find <, stop
			if (!find('<'))
			{
				break;
			}

			int current_tag_start = position;

			shared_ptr<HtmlTag> tag = parse_tag();
			if (tag == NULL)
			{
				skip_forward(1);
				continue;
			}

			if (tag->self_closed())
			{
				continue;
			}

			if (tag->name() == opening_tag->name())
			{
				if (tag->closing())
				{
					depth--;

					if (depth == 0)
					{
						skip_linespace();

						token->tag(opening_tag);
						token->end(position);

						InlineProcessor processor(input, content_start, current_tag_start - content_start);
						token->children(processor.process());

						return true;
					}
				}
				else
				{
					depth++;
				}
			}
		}

		return false;
	}

	shared_ptr<HtmlTag> InlineProcessor::parse_tag()
	{
		int start = position;

		if (current() != '<')
		{
			return NULL;
		}

		// skip '<'
		skip_forward(1);

		if (does_match("!--"))
		{
			skip_forward(3);

			mark();

			if (find("-->"))
			{
				HtmlTag t("!");
				t.attributes()["content"] = extract();
				t.self_closed(true);

				skip_forward(3);
			}
		}

		bool closing = false;

		if (current() == '/')
		{
			closing = true;
			skip_forward(1);
		}

		string name = extract_word_block();
		if (is_null_or_whitespace(name))
		{
			return NULL;
		}

		shared_ptr<HtmlTag> tag{ new HtmlTag(name) };
		tag->closing(closing);

		if (closing)
		{
			// There is an attribute in a closing tag!
			if (current() != '>')
			{
				return NULL;
			}

			skip_forward(1);
			return tag;
		}

		while (!at_file_end())
		{
			skip_whitespace();

			if (does_match("/>"))
			{
				tag->self_closed(true);
				skip_forward(2);
				return tag;
			}

			if (does_match(">"))
			{
				skip_forward(1);
				return tag;
			}

			string attribute_name = extract_word_block();
			if (is_null_or_whitespace(attribute_name))
			{
				return NULL;
			}

			skip_whitespace();

			// check if the attribute has value, like src=""
			// or doesn't have a value, like autofocus
			bool has_value = does_match('=');

			if (has_value)
			{
				skip_forward(1);
				skip_whitespace();

				bool has_quote = does_match('"') || does_match("'");

				if (has_quote)
				{
					char quote = current();

					skip_forward(1);
					mark();

					if (!find(quote))
					{
						return NULL;
					}

					tag->attributes()[attribute_name] = extract();

					skip_forward(1);
				}
				else
				{
					mark();

					while (!at_file_end() &&
						!(is_linespace(current()) || current() == '>' || current() == '/'))
					{
						skip_forward(1);
					}

					if (!at_file_end())
					{
						tag->attributes()[attribute_name] = extract();
					}
				}
			}
			else
			{
				tag->attributes()[attribute_name] = "";
			}
		}

		return NULL;
	}

	shared_ptr<Token> InlineProcessor::make_link_or_image_token()
	{
		int start = position;
		bool is_link = true;

		if (does_match('!'))
		{
			is_link = false;
			skip_forward(1);
		}

		// skip first '['
		skip_forward(1);

		mark();
		int depth = 1;

		while (!at_file_end())
		{
			if (current() == '[')
			{
				depth++;
			}
			else if (current() == ']')
			{
				depth--;

				if (depth == 0)
				{
					break;
				}
			}

			skip_forward(1);
		}

		if (at_file_end())
		{
			return NULL;
		}

		// first extract without last ']'
		string link_text = extract();

		// and then skip last ']'
		skip_forward(1);

		if (link_text[0] == '[' &&
			link_text[link_text.length() - 1] == ']')
		{
			link_text = link_text.substr(1, link_text.length() - 2);

			return shared_ptr<LinkToken>(new LinkToken(input, start, position - start, link_text, link_text, true));
		}
		else
		{
			if (does_match('('))
			{
				skip_forward(1);

				mark();
				depth = 1;

				while (!at_file_end())
				{
					if (current() == '(')
					{
						depth++;
					}
					else if (current() == ')')
					{
						depth--;

						if (depth == 0)
						{
							break;
						}
					}

					skip_forward(1);
				}

				if (at_file_end())
				{
					return NULL;
				}

				// first extract without trailing last ')'
				string link_url = extract();

				// and then skip last ')'
				skip_forward(1);

				if (is_link)
				{
					bool is_internal = is_safe_url(link_url);
					return shared_ptr<LinkToken>(new LinkToken(input, start, position - start, link_text, link_url, is_internal));
				}
				else
				{
					return shared_ptr<ImageToken>(new ImageToken(input, start, position - start, link_text, link_url));
				}
			}
		}

		return NULL;
	}

	// TODO: clean
	vector<shared_ptr<Token>> InlineProcessor::integrate(vector<shared_ptr<Token>>& tokens)
	{
		// Handle EmphasisParseTokens
		int depth = 0;
		bool internal_opening = false;

		map<int, vector<array<shared_ptr<EmphasisParseToken>, 2>>> depth_tokens_dic;
		
		for (int i = 0; i < tokens.size(); ++i)
		{
			// If the token is not a EmphasisParseToken
			if (typeid(*tokens[i]) != typeid(EmphasisParseToken))
			{
				internal_opening = false;
				continue;
			}

			shared_ptr<EmphasisParseToken> token = std::dynamic_pointer_cast<EmphasisParseToken>(tokens[i]);

			if (token->type() == TokenType::EmphasisOpen ||
				(token->type() == TokenType::EmphasisInternal && (depth == 0 || internal_opening)))
			{
				depth++;

				array<shared_ptr<EmphasisParseToken>, 2> depth_tokens = {token};

				depth_tokens_dic[depth].push_back(depth_tokens);

				if (token->type() == TokenType::EmphasisInternal)
				{
					internal_opening = true;
				}
				else
				{
					internal_opening = false;
				}
			}
			else
			{
				if (depth == 0)
				{
					continue;
				}

				depth_tokens_dic[depth][depth_tokens_dic[depth].size() - 1][1] = token;

				depth--;
			}
		}

		return create_emphasis_hierarchy(tokens, depth_tokens_dic, 1, 0);
	}

	vector<shared_ptr<Token>> InlineProcessor::create_emphasis_hierarchy(vector<shared_ptr<Token>> tokens, map<int, vector<array<shared_ptr<EmphasisParseToken>, 2>>> depth_tokens_dic, int depth, int index)
	{
		auto token_infos = depth_tokens_dic[depth];

		for (int i = index; i < token_infos.size(); ++i)
		{
			auto token_info = token_infos[i];

			auto start_token = token_info[0];
			auto end_token = token_info[1];

			if (end_token == NULL)
			{
				return create_emphasis_hierarchy(tokens, depth_tokens_dic, depth + 1, i);
			}

			int content_start = start_token->start() + 1;
			int content_end = end_token->end() - 1;

			bool is_strong = false;

			int start_index = index_of(tokens, start_token);
			int end_index = index_of(tokens, end_token);
			int length = end_index - start_index + 1;

			if (input[content_start] == input[content_start - 1] &&
				input[content_end] == input[content_end - 1])
			{
				content_start++;
				content_end--;

				remove(tokens, start_index + 1);
				remove(tokens, end_index - 1);

				length -= 2;

				is_strong = true;
			}

			shared_ptr<EmphasisToken> token(new EmphasisToken(input, content_start, content_end - content_start, is_strong));
			token->children(integrate(sub_vector(tokens, start_index + 1, length - 2)));

			remove_range(tokens, start_index, length);
			tokens.insert(tokens.begin() + start_index, token);
		}

		return tokens;
	}
}