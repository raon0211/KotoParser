#ifndef BLOCKS_H
#define BLOCKS_H

#include "block.h"

namespace kotoparser
{
	class HeadingBlock : public Block
	{
	public:
		int level() const { return _level; }
		HeadingBlock& level(int new_level) { _level = new_level; return *this; }

		HeadingBlock(wstring buffer, int level) : Block(buffer)
		{
			_type = block_type::heading;
			_level = level;
		}

		virtual wstring render();
	private:
		int _level;
	};

	class BlockquoteBlock : public Block
	{
	public:
		BlockquoteBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::blockquote;
		}

		virtual wstring render();		
	};

	class CodeBlock : public Block
	{
	public:
		wstring language() const { return _language; }
		CodeBlock& language(wstring new_language) { _language = new_language; return *this; }

		CodeBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::code;
		}

		virtual wstring render();
	private:
		wstring _language;
	};

	class DefinitionListBlock : public Block
	{
	public:
		DefinitionListBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::definition_list;
		}

		virtual wstring render();
	};

	class TermBlock : public Block
	{
	public:
		TermBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::term;
		}

		virtual wstring render();
	};

	class DefinitionBlock : public Block
	{
	public:
		DefinitionBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::definition;
		}

		virtual wstring render();
	};

	class ListBlock : public Block
	{
	public:
		ListBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::list;
		}

		virtual wstring render();
	};

	class ListItemBlock : public Block
	{
	public:
		bool is_ordered() const { return _is_ordered; }
		ListItemBlock& is_ordered(bool new_is_ordered) { _is_ordered = new_is_ordered; return *this; }

		int indent() const { return _indent; }
		ListItemBlock& indent(int new_indent) { _indent = new_indent; return *this; }

		int level() const { return _level; }
		ListItemBlock& level(int new_level) { _level = new_level; return *this; }

		ListItemBlock(wstring buffer, bool is_ordered, int indent) : Block(buffer)
		{
			_type = block_type::list_item;
			_is_ordered = is_ordered;
			_indent = indent;
		}

		virtual wstring render();

	private:
		bool _is_ordered;
		int _indent;
		int _level;
	};

	class ParagraphBlock : public Block
	{
	public:
		ParagraphBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::paragraph;
		}

		virtual wstring render();
	};

	class PlainBlock : public Block
	{
	public:
		PlainBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::plain;
		}

		virtual wstring render();
	};

	class BlankBlock : public Block
	{
	public:
		BlankBlock(wstring buffer) : Block(buffer)
		{
			_type = block_type::blank;
		}

		virtual wstring render();
	};
}

#endif