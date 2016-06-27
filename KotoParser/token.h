#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using std::wstring;

namespace kotoparser
{
	enum class TokenType
	{
		Emphasis, EmphasisOpen, EmphasisClose, EmphasisInternal,
		Code,
		Text,
		Html, Tag,
		Link, Img, Footnote,
		Br
	};

	class Token
	{
	public:
		TokenType type() const { return _type; }
		Token& type(TokenType new_type) { _type = new_type; return *this; }
		
		wstring buffer() const { return _buffer; }
		Token& buffer(wstring new_buffer) { _buffer = new_buffer; return *this; }

		int start() const { return _start; }
		Token& start(int new_start) { _start = new_start; return *this; }

		int end() const { return _start + _length; }
		Token& end(int new_end) { _length = new_end - _start; return *this; }

		int length() const { return _length; }
		Token& length(int new_length) { _length = new_length; return *this; }

		wstring content() const
		{
			return _buffer.substr(_start, _length);
		}

		Token(wstring buffer)
		{
			_buffer = buffer;
		}

		Token(wstring buffer, int start, int length)
		{
			_buffer = buffer;
			_start = start;
			_length = length;
		}

		virtual wstring render();

	protected:
		TokenType _type;
		wstring _buffer;
		int _start;
		int _length;
	};
}

#endif