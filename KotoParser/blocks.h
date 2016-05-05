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