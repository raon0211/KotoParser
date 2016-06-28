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

		HeadingBlock(string buffer, int level) : Block(buffer)
		{
			_type = BlockType::Heading;
			_level = level;
		}

		virtual string render();
	private:
		int _level;
	};

	class BlockquoteBlock : public Block
	{
	public:
		BlockquoteBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Blockquote;
		}

		virtual string render();		
	};

	class CodeBlock : public Block
	{
	public:
		string language() const { return _language; }
		CodeBlock& language(string new_language) { _language = new_language; return *this; }

		CodeBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Code;
		}

		virtual string render();
	private:
		string _language;
	};

	class DefinitionListBlock : public Block
	{
	public:
		DefinitionListBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::DefinitionList;
		}

		virtual string render();
	};

	class TermBlock : public Block
	{
	public:
		TermBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Term;
		}

		virtual string render();
	};

	class DefinitionBlock : public Block
	{
	public:
		DefinitionBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Definition;
		}

		virtual string render();
	};

	class TableBlock : public Block
	{
	public:
		TableBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Table;
		}

		virtual string render();
	};

	class TableRowBlock : public Block
	{
	public:
		TableRowBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::TableRow;
		}

		virtual string render();
	};

	enum class TableDataAlignment
	{
		Left, Center, Right
	};

	class TableDataBlock : public Block
	{
	public:
		bool is_header() const { return _is_header; }
		TableDataBlock& is_header(bool new_is_header) { _is_header = new_is_header; return *this; }
		
		int colspan() const { return _colspan; }
		TableDataBlock& colspan(int new_colspan) { _colspan = new_colspan; return *this; }

		int rowspan() const { return _rowspan; }
		TableDataBlock& rowspan(int new_rowspan) { _rowspan = new_rowspan; return *this; }

		TableDataAlignment alignment() const { return _alignment; }
		TableDataBlock& alignment(TableDataAlignment new_alignment) { _alignment = new_alignment; return *this; }

		TableDataBlock(string buffer, bool is_header) : Block(buffer)
		{
			_type = BlockType::TableData;
			_is_header = is_header;
			_colspan = 1;
			_rowspan = 1;
		}

		virtual string render();

	private:
		bool _is_header;
		int _colspan;
		int _rowspan;
		TableDataAlignment _alignment;
	};

	class ListBlock : public Block
	{
	public:
		ListBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::List;
		}

		virtual string render();
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

		ListItemBlock(string buffer, bool is_ordered, int indent) : Block(buffer)
		{
			_type = BlockType::ListItem;
			_is_ordered = is_ordered;
			_indent = indent;
		}

		virtual string render();

	private:
		bool _is_ordered;
		int _indent;
		int _level;
	};

	class ParagraphBlock : public Block
	{
	public:
		ParagraphBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Paragraph;
		}

		virtual string render();
	};
	
	class HtmlBlock : public Block
	{
	public:
		shared_ptr<HtmlTag> tag() const { return _tag; }
		HtmlBlock& tag(shared_ptr<HtmlTag> new_tag) { _tag = new_tag; return *this; }

		HtmlBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Html;
		}

		virtual string render();
	private:
		shared_ptr<HtmlTag> _tag;
	};

	class PlainBlock : public Block
	{
	public:
		PlainBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Plain;
		}

		virtual string render();
	};

	class BlankBlock : public Block
	{
	public:
		BlankBlock(string buffer) : Block(buffer)
		{
			_type = BlockType::Blank;
		}

		virtual string render();
	};
}

#endif