#include "block_processor.h"
#include <vector>
#include <string>
#include "blocks.h"

using std::reference_wrapper;
using std::vector;
using std::wstring;

namespace kotoparser
{
	vector<Block *> BlockProcessor::process()
	{
		// 일단 Block을 모두 파싱하고
		vector<Block *> blocks = parse();

		// 이제 있는 Block은 줄별로 파싱되었으므로
		// Paragraph - Paragraph - Blank - Paragraph - ListItem - ListItem - Heading 을
		// Paragraph - Paragraph - List - Heading 으로 변환

		// blocks = integrate(blocks);

		return blocks;
	}

	vector<Block *> BlockProcessor::parse()
	{
		vector<Block *> blocks;

		int start = position;

		while (!at_file_end())
		{
			Block *block = NULL;

			if (current() == '#')
			{
				int level = 0;

				do
				{
					++level;
					skip_forward(1);
				}
				while (current() == '#');

				if (level > 3)
				{
					level = 3;
				}

				block = new HeadingBlock(input, level);
				block->buffer(input);

				skip_linespace();

				block->start(position);
				skip_to_line_end();
				block->end(position);
			}

			if (block != NULL)
			{
				blocks.push_back(block);

				skip_line_end();
				start = position;
			}
			else
			{
				skip_forward(1);
				skip_line_end();
			}
		}

		return blocks;
	}
}