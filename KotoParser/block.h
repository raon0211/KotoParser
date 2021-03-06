#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using std::shared_ptr;

namespace kotoparser
{
	enum class BlockType
	{
		Blank,
		Heading,
		Blockquote,
		Code,
		Table, TableRow, TableData,
		Html,
		List, ListItem,
		DefinitionList, Term, Definition,
		Paragraph,
		Plain
	};

	class Block
	{
	public:
		BlockType type() const { return _type; }
		Block& type(BlockType new_type) { _type = new_type; return *this; }

		vector<shared_ptr<Block>>& children() { return _children; }
		Block& children(vector<shared_ptr<Block>> new_children) { _children = new_children; return *this; }

		string buffer() const { return _buffer; }
		Block& buffer(string new_buffer) { _buffer = new_buffer; return *this; }

		int start() const { return _start; }
		Block& start(int new_start) { _start = new_start; return *this; }

		int end() const { return _start + _length; }
		Block& end(int new_end) { _length = new_end - _start; return *this; }

		int length() const { return _length; }
		Block& length(int new_length) { _length = new_length; return *this; }

		string content() const 
		{ 
			return _buffer.substr(_start, _length);
		};

		Block(string buffer)
		{
			_type = BlockType::Paragraph;
			_buffer = buffer;
		}

		virtual string render();
	protected:
		BlockType _type;
		vector<shared_ptr<Block>> _children;
		string _buffer;
		int _start;
		int _length;
	};
}

#endif