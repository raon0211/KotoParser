#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::wstring;
using std::shared_ptr;

namespace kotoparser
{
	enum class block_type
	{
		blank,
		heading,
		blockquote,
		code,
		table,
		html,
		list, list_item,
		definition_list, term, definition,
		paragraph,
		plain
	};

	class Block
	{
	public:
		block_type type() const { return _type; }
		Block& type(block_type new_type) { _type = new_type; return *this; }

		vector<shared_ptr<Block>>& children() { return _children; }
		Block& children(vector<shared_ptr<Block>> new_children) { _children = new_children; return *this; }

		wstring buffer() const { return _buffer; }
		Block& buffer(wstring new_buffer) { _buffer = new_buffer; return *this; }

		int start() const { return _start; }
		Block& start(int new_start) { _start = new_start; return *this; }

		int end() const { return _start + _length; }
		Block& end(int new_end) { _length = new_end - _start; return *this; }

		int length() const { return _length; }
		Block& length(int new_length) { _length = new_length; return *this; }

		wstring content() const { 
			return _buffer.substr(_start, _length);
		};

		Block(wstring buffer)
		{
			_type = block_type::paragraph;
			_buffer = buffer;
		}

		virtual wstring render();
	protected:
		block_type _type;
		vector<shared_ptr<Block>> _children;
		wstring _buffer;
		int _start;
		int _length;
	};
}

#endif