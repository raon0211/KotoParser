#ifndef TOKENS_H
#define TOKENS_H

#include <vector>
#include <memory>
#include "token.h"
#include "html_tag.h"

using std::vector;
using std::shared_ptr;

namespace kotoparser
{
	class EmphasisToken : public Token
	{
	public:
		bool is_strong() const { return _is_strong; }
		EmphasisToken& is_strong(bool new_is_strong) { _is_strong = new_is_strong; return *this; }

		vector<shared_ptr<Token>>& children() { return _children; }
		EmphasisToken& children(vector<shared_ptr<Token>>& new_children) { _children = new_children; return *this; }

		EmphasisToken(wstring buffer, int start, int length, bool is_strong) : Token(buffer, start, length)
		{
			_type = TokenType::Emphasis;
			_is_strong = is_strong;
		}

		virtual wstring render();

	private:
		bool _is_strong;
		vector<shared_ptr<Token>> _children;
	};

	enum class EmphasisType
	{
		Opening,
		Closing,
		Internal
	};

	class EmphasisParseToken : public Token
	{
	public:
		EmphasisParseToken(wstring buffer, int start, int length, EmphasisType is_opening) : Token(buffer, start, length)
		{
			switch (is_opening)
			{
				case EmphasisType::Opening:
					_type = TokenType::EmphasisOpen;
					break;
				case EmphasisType::Closing:
					_type = TokenType::EmphasisClose;
					break;
				case EmphasisType::Internal:
					_type = TokenType::EmphasisInternal;
					break;
			}
		}

		virtual wstring render();
	};

	class CodeToken : public Token
	{
	public: 
		CodeToken(wstring buffer, int start, int length) : Token(buffer, start, length)
		{
			_type = TokenType::Code;
		}

		virtual wstring render();
	};

	class HtmlToken : public Token
	{
	public:
		shared_ptr<HtmlTag> tag() const { return _tag; }
		HtmlToken& tag(shared_ptr<HtmlTag> new_tag) { _tag = new_tag; return *this; }

		vector<shared_ptr<Token>>& children() { return _children; }
		HtmlToken& children(vector<shared_ptr<Token>>& new_children) { _children = new_children; return *this; }

		HtmlToken(wstring buffer) : Token(buffer)
		{
			_type = TokenType::Html;
		}

		virtual wstring render();

	private:
		shared_ptr<HtmlTag> _tag;
		vector<shared_ptr<Token>> _children;
	};

	class LinkToken : public Token
	{
	public:
		wstring text() const { return _text; }
		LinkToken& text(wstring& new_text) { _text = new_text; return *this; }

		wstring url() const { return _url; }
		LinkToken& url(wstring& new_url) { _text = new_url; return *this; }

		bool is_internal() const { return _is_internal; }
		LinkToken& is_internal(bool new_is_internal) { _is_internal = new_is_internal; return *this; }

		LinkToken(wstring buffer, int start, int length, wstring text, wstring url, bool is_internal) : Token(buffer, start, length)
		{
			_type = TokenType::Link;
			_text = text;
			_url = url;
			_is_internal = is_internal;
		}

		virtual wstring render();
	private:
		wstring _text;
		wstring _url;
		bool _is_internal;
	};

	class ImageToken : public Token
	{
	public:
		wstring text() const { return _text; }
		ImageToken& text(wstring& new_text) { _text = new_text; return *this; }

		wstring url() const { return _url; }
		ImageToken& url(wstring& new_url) { _text = new_url; return *this; }

		ImageToken(wstring buffer, int start, int length, wstring text, wstring url) : Token(buffer, start, length)
		{
			_type = TokenType::Img;
			_text = text;
			_url = url;
		}

		virtual wstring render();
	private:
		wstring _text;
		wstring _url;
	};

	class TextToken : public Token
	{
	public:
		TextToken(wstring buffer, int start, int length) : Token(buffer, start, length)
		{
			_type = TokenType::Text;
		}

		virtual wstring render();
	};
}

#endif