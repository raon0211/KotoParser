#include <vector>
#include "string_scanner.h"
#include "block.h"

using std::vector;

#ifndef BLOCK_PROCESSOR_H
#define BLOCK_PROCESSOR_H

namespace kotoparser
{
	class BlockProcessor : public StringScanner
	{
	public:
		BlockProcessor(wstring input) : StringScanner(input) { }
		BlockProcessor(wstring input, int start, int length) : StringScanner(input, start, length) { }
	
		vector<Block *> process();
	
	private:
		vector<Block *> parse();
		vector<Block *> make_interspace_block();
		int check_indent();
		
		vector<Block *> integrate(vector<Block>& blocks);
		void add_integrated_block(vector<Block>& result, vector<Block>& lines);
		Block& make_blocks_from_lines(vector<Block>& lines);
		vector<Block *> set_list_level(vector<Block>& lines);
	};
}
#endif