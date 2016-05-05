#include "kotoparser.h"
#include <string>
#include "block.h"
#include "block_processor.h"

using namespace std;

namespace kotoparser
{
	wstring transform(wstring input)
	{
		wstring result;

		auto blocks = BlockProcessor(input).process();

		for (shared_ptr<Block>block : blocks)
		{
			result += block->render();
		}

		return result;
	}
}