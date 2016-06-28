#include "table_processor.h"

#include <vector>
#include <memory>
#include "block.h"
#include "blocks.h"
#include "utilities.h"

using std::vector;
using std::shared_ptr;

namespace kotoparser
{
	vector<shared_ptr<Block>> TableProcessor::process()
	{
		vector<shared_ptr<Block>> blocks;

		while (!at_file_end())
		{
			shared_ptr<TableDataBlock> block;

			if (is_table_separator(current()))
			{
				block.reset(new TableDataBlock(input, current() == '^'));
				skip_forward(1);

				int start_indent = 0;
				while (is_linespace(current()))
				{
					skip_forward(1);
					start_indent++;
				}

				block->start(position);

				while (!is_table_separator(current()) &&
					   !at_file_end())
				{
					skip_forward(1);
				}

				int end = position;
				skip_forward(-1);

				int end_indent = 0;
				while (is_linespace(current()))
				{
					skip_forward(-1);
					end_indent++;
				}

				if (start_indent >= 2 && end_indent >= 2)
				{
					block->alignment(TableDataAlignment::Center);
				}
				else if (start_indent >= 2)
				{
					block->alignment(TableDataAlignment::Right);
				}
				else
				{
					block->alignment(TableDataAlignment::Left);
				}

				block->end(position + 1);

				if (is_null_or_whitespace(block->content()))
				{
					auto last_block = std::dynamic_pointer_cast<TableDataBlock>(blocks[blocks.size() - 1]);
					int colspan = last_block->colspan();
					last_block->colspan(colspan + 1);

					block = NULL;
				}

				position = end;
			}

			if (block != NULL)
			{
				blocks.push_back(block);
			}
		}

		return blocks;
	}

	bool TableProcessor::is_table_separator(char c)
	{
		return c == '^' || c == '|';
	}
}