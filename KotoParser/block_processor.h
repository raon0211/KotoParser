#ifndef BLOCK_PROCESSOR_H
#define BLOCK_PROCESSOR_H

#include <vector>
#include <memory>
#include "string_scanner.h"
#include "block.h"
#include "blocks.h"

using std::vector;
using std::shared_ptr;

namespace kotoparser
{
	class BlockProcessor : public StringScanner
	{
	public:
		BlockProcessor(wstring input) : StringScanner(input) { }
		BlockProcessor(wstring input, int start, int length) : StringScanner(input, start, length) { }
	
		vector<shared_ptr<Block>> process();
	
	private:
		vector<shared_ptr<Block>> parse();
		vector<shared_ptr<Block>> make_interspace_block();
		int check_indent();
		bool make_code_block(shared_ptr<CodeBlock> block);
		bool make_html(shared_ptr<HtmlBlock> block);
		shared_ptr<HtmlTag> parse_tag();
		
		vector<shared_ptr<Block>> integrate(vector<shared_ptr<Block>> blocks);
		void add_integrated_block(vector<shared_ptr<Block>>& result, vector<shared_ptr<Block>>& lines);
		shared_ptr<Block> make_blocks_from_lines(vector<shared_ptr<Block>>& lines);
		vector<shared_ptr<Block>> set_list_level(vector<shared_ptr<Block>>& lines);
		void make_list_hierarchy(shared_ptr<ListBlock> list);
	};
}
#endif