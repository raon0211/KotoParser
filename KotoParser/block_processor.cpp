#include "block_processor.h"
#include <vector>
#include <string>
#include <memory>
#include "blocks.h"

using std::reference_wrapper;
using std::vector;
using std::wstring;
using std::shared_ptr;

namespace kotoparser
{
	vector<shared_ptr<Block>> BlockProcessor::process()
	{
		// 일단 Block을 모두 파싱하고
		vector<shared_ptr<Block>> blocks = parse();

		// 이제 있는 Block은 줄별로 파싱되었으므로
		// Paragraph - Paragraph - Blank - Paragraph - ListItem - ListItem - Heading 을
		// Paragraph - Paragraph - List - Heading 으로 변환

		// blocks = integrate(blocks);

		return blocks;
	}

	vector<shared_ptr<Block>> BlockProcessor::parse()
	{
		vector<shared_ptr<Block>> blocks;

		int start = position;

		while (!at_file_end())
		{
			int line_start = position;
			shared_ptr<Block> block = NULL;

			// # Heading
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

				block.reset(new HeadingBlock(input, level));

				skip_linespace();

				block->start(position);
				skip_to_line_end();
				block->end(position);
			}
			// > Blockquote
			else if (current() == '>')
			{
				block.reset(new BlockquoteBlock(input));

				skip_forward(1);
				skip_linespace();

				block->start(position);
				skip_to_line_end();
				block->end(position);
			}
			// ```
			// code
			// ```
			else if (current() == '`')
			{
				block.reset(new CodeBlock(input));
				block->start(position);

				if (!make_code_block(std::dynamic_pointer_cast<CodeBlock>(block)))
				{
					block = NULL;
					position = line_start;
				}
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

	bool BlockProcessor::make_code_block(shared_ptr<CodeBlock> block)
	{
		wchar_t delim = current();

		// Check if there are three `s
		mark();

		while (current() == delim)
		{
			skip_forward(1);
		}

		wstring fence = extract();

		if (fence.length() < 3)
		{
			return false;
		}

		skip_linespace();

		// Set language like ```ruby
		if (!at_line_end())
		{
			mark();
			skip_to_line_end();
			wstring language = extract();

			block->language(language);
		}

		skip_line_end();

		int start = position;

		if (!find(fence) || !is_line_end(char_at_offset(-1)))
		{
			return false;
		}

		int end = position;

		skip_forward(fence.length());
		skip_linespace();

		if (!at_line_end())
		{
			return false;
		}

		// 뒤에 잡다한 LineEnd 제거
		if (input[end - 1] == '\r' && input[end - 2] == '\n')
		{
			end -= 2;
		}
		else if (input[end - 1] == '\n' && input[end - 2] == '\r')
		{
			end -= 2;
		}
		else
		{
			end--;
		}

		shared_ptr<PlainBlock> child{ new PlainBlock(input) };
		child->start(start);
		child->end(end);

		block->children().push_back(child);
		block->end(end);

		return true;
	}
}