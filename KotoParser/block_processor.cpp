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
		// �ϴ� Block�� ��� �Ľ��ϰ�
		vector<Block *> blocks = parse();

		// ���� �ִ� Block�� �ٺ��� �Ľ̵Ǿ����Ƿ�
		// Paragraph - Paragraph - Blank - Paragraph - ListItem - ListItem - Heading ��
		// Paragraph - Paragraph - List - Heading ���� ��ȯ

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