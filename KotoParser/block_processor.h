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
		
		vector<shared_ptr<Block>> integrate(vector<Block>& blocks);
		void add_integrated_block(vector<Block>& result, vector<Block>& lines);
		Block& make_blocks_from_lines(vector<Block>& lines);
		vector<shared_ptr<Block>> set_list_level(vector<Block>& lines);
	};
}
#endif