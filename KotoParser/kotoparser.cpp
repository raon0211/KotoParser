#include "kotoparser.h"
#include <string>
#include "block.h"
#include "block_processor.h"

using namespace std;

namespace kotoparser
{
	wstring KotoParser::transform(wstring input)
	{
		wstring result;

		auto blocks = BlockProcessor(input).process();

		for (Block *block : blocks)
		{
			result += block->render();
		}

		return result;
	}
}