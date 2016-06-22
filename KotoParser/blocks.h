#ifndef BLOCKS_H
#define BLOCKS_H

#include "block.h"
#include "html_tag.h"

namespace kotoparser
{
	class HeadingBlock : public Block
	{
	public:
		int level() const { return _level; }
		HeadingBlock& level(int new_level) { _level = new_level; return *this; }

		HeadingBlock(wstring buffer, int level) : Block(buffer)
		{
			_type = BlockType::Heading;
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
			_type = BlockType::Blockquote;
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
			_type = BlockType::Code;
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
			_type = BlockType::DefinitionList;
		}

		virtual wstring render();
	};

	class TermBlock : public Block
	{
	public:
		TermBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::Term;
		}

		virtual wstring render();
	};

	class DefinitionBlock : public Block
	{
	public:
		DefinitionBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::Definition;
		}

		virtual wstring render();
	};

	class ListBlock : public Block
	{
	public:
		ListBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::List;
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
			_type = BlockType::ListItem;
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
			_type = BlockType::Paragraph;
		}

		virtual wstring render();
	};
	
	class HtmlBlock : public Block
	{
	public:
		shared_ptr<HtmlTag> tag() const { return _tag; }
		HtmlBlock& tag(shared_ptr<HtmlTag> new_tag) { _tag = new_tag; return *this; }

		HtmlBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::Html;
		}

		virtual wstring render();
	private:
		shared_ptr<HtmlTag> _tag;
	};

	class PlainBlock : public Block
	{
	public:
		PlainBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::Plain;
		}

		virtual wstring render();
	};

	class BlankBlock : public Block
	{
	public:
		BlankBlock(wstring buffer) : Block(buffer)
		{
			_type = BlockType::Blank;
		}

		virtual wstring render();
	};
}

#endif