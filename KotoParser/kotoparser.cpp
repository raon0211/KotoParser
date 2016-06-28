#include "kotoparser.h"
#include <string>
#include "block.h"
#include "block_processor.h"

using namespace std;

namespace kotoparser
{
	string transform(string input)
	{
		string result;

		auto blocks = BlockProcessor(input).process();

		for (shared_ptr<Block> block : blocks)
		{
			result += block->render();
		}

		return result;
	}
}