#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using std::string;

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
		
		string buffer() const { return _buffer; }
		Token& buffer(string new_buffer) { _buffer = new_buffer; return *this; }

		int start() const { return _start; }
		Token& start(int new_start) { _start = new_start; return *this; }

		int end() const { return _start + _length; }
		Token& end(int new_end) { _length = new_end - _start; return *this; }

		int length() const { return _length; }
		Token& length(int new_length) { _length = new_length; return *this; }

		string content() const
		{
			return _buffer.substr(_start, _length);
		}

		Token(string buffer)
		{
			_buffer = buffer;
		}

		Token(string buffer, int start, int length)
		{
			_buffer = buffer;
			_start = start;
			_length = length;
		}

		virtual string render();

	protected:
		TokenType _type;
		string _buffer;
		int _start;
		int _length;
	};
}

#endif