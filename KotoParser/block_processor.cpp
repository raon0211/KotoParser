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
		blocks = integrate(blocks);

		return blocks;
	}

	vector<shared_ptr<Block>> BlockProcessor::parse()
	{
		vector<shared_ptr<Block>> blocks;

		int start = position;

		while (!at_file_end())
		{
			int line_start = position;

			block_type last_block_type = blocks.size() > 0 ? blocks[blocks.size() - 1]->type() : block_type::blank;
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
			else if (current() == ';' && is_line_end(char_at_offset(-1)))
			{
				skip_forward(1);
				skip_linespace();

				block.reset(new TermBlock(input));
				block->start(position);

				while (current() != ':' && !at_line_end())
				{
					skip_forward(1);
				}

				block->end(position);
			}
			else if (current() == ':' && (last_block_type == block_type::term || last_block_type == block_type::definition))
			{
				skip_forward(1);
				skip_linespace();

				block.reset(new DefinitionBlock(input));
				block->start(position);

				while (current() != ':' && !at_line_end())
				{
					skip_forward(1);
				}

				block->end(position);
			}
			else if ((current() == '*' || current() == '-' || current() == '+') && is_linespace(char_at_offset(1)) && char_is_first_of_line())
			{
				skip_forward(1);
				skip_linespace();

				block.reset(new ListItemBlock(input, false, check_indent()));
				block->start(position);

				skip_to_line_end();

				block->end(position);
			}
			else if (iswdigit(current()) && char_is_first_of_line())
			{
				do
				{
					skip_forward(1);
				}
				while (iswdigit(current()));

				if (does_match('.') && is_linespace(char_at_offset(1)))
				{
					skip_forward(2);

					block.reset(new ListItemBlock(input, true, check_indent()));
					block->start(position);

					skip_to_line_end();
					
					block->end(position);
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

	int BlockProcessor::check_indent()
	{
		int indent = 0;
		int start = position;

		skip_forward(-1);

		while (!at_line_end())
		{
			if (!is_linespace(current()))
			{
				indent = 0;
			}
			else
			{
				++indent;
			}

			skip_forward(-1);
		}
		
		position = start;
		return indent;
	}

	vector<shared_ptr<Block>> BlockProcessor::integrate(vector<shared_ptr<Block>> blocks)
	{
		vector<shared_ptr<Block>> result;

		vector<shared_ptr<Block>> lines;

		for (shared_ptr<Block> block : blocks)
		{
			block_type lines_type = lines.size() > 0 ? lines[0]->type() : block_type::blank;

			switch (block->type())
			{
			case block_type::blank:
				add_integrated_block(result, lines);
				break;

			case block_type::paragraph:
				if (lines_type == block_type::blank ||
					lines_type == block_type::paragraph ||
					lines_type == block_type::blockquote ||
					lines_type == block_type::list_item)
				{
					lines.push_back(block);
					break;
				}

				add_integrated_block(result, lines);
				lines.push_back(block);
				break;

			case block_type::list_item:
				if (lines_type == block_type::blank)
				{
					lines.push_back(block);
					break;
				}
				else if (lines_type == block_type::list_item)
				{
					auto first = std::dynamic_pointer_cast<ListItemBlock>(lines[0]);
					auto current = std::dynamic_pointer_cast<ListItemBlock>(block);

					if (first->is_ordered() == current->is_ordered())
					{
						lines.push_back(block);
						break;
					}
				}

				add_integrated_block(result, lines);
				lines.push_back(block);
				break;

			case block_type::term:
			case block_type::definition:
				if (lines_type == block_type::blank ||
					lines_type == block_type::term ||
					lines_type == block_type::definition)
				{
					lines.push_back(block);
					break;
				}

				add_integrated_block(result, lines);
				lines.push_back(block);
				break;

			default:
				add_integrated_block(result, lines);
				result.push_back(block);
				break;
			}
		}

		add_integrated_block(result, lines);

		return result;
	}

	void BlockProcessor::add_integrated_block(vector<shared_ptr<Block>>& result, vector<shared_ptr<Block>>& lines)
	{
		auto integrated_block = make_blocks_from_lines(lines);

		if (integrated_block != NULL)
		{
			result.push_back(integrated_block);
		}
	}

	shared_ptr<Block> BlockProcessor::make_blocks_from_lines(vector<shared_ptr<Block>>& lines)
	{
		if (lines.size() == 0)
		{
			return NULL;
		}

		switch (lines[0]->type())
		{
		case block_type::paragraph:
		{
			shared_ptr<ParagraphBlock> p{ new ParagraphBlock(lines[0]->buffer()) };
			p->start(lines[0]->start()).end(lines[lines.size() - 1]->end());;

			lines.clear();

			return p;
		}

		case block_type::list_item:
		{
			shared_ptr<ListBlock> list{ new ListBlock(lines[0]->buffer()) };
			list->start(lines[0]->start()).end(lines[lines.size() - 1]->end());
			list->children(set_list_level(lines));

			make_list_hierarchy(list);

			lines.clear();

			return list;
		}
		
		case block_type::term:
		case block_type::definition:
		{
			shared_ptr<DefinitionListBlock> def{ new DefinitionListBlock(lines[0]->buffer()) };
			def->start(lines[0]->start()).end(lines[lines.size() - 1]->end());

			for (auto block : lines)
			{
				def->children().push_back(block);
			}

			lines.clear();

			return def;
		}
		}

		return NULL;
	}

	vector<shared_ptr<Block>> BlockProcessor::set_list_level(vector<shared_ptr<Block>>& lines)
	{
		// First, compound list_items and paragraphs to only list_items
		std::dynamic_pointer_cast<ListItemBlock>(lines[0])->level(1);

		for (int i = 1; (unsigned)i < lines.size(); ++i)
		{
			// Compound if paragraph
			if (lines[i]->type() == block_type::paragraph)
			{
				lines[i - 1]->end(lines[i]->end());
				lines.erase(lines.begin() + i);
				i--;

				continue;
			}

			auto prev = std::dynamic_pointer_cast<ListItemBlock>(lines[i - 1]);
			auto current = std::dynamic_pointer_cast<ListItemBlock>(lines[i]);

			if (prev->indent() > current->indent())
			{
				current->level(prev->level() - 1);

				if (current->level() < 1)
				{
					current->level(1);
				}
			}
			else if (prev->indent() == current->indent())
			{
				current->level(prev->level());
			}
			else
			{
				current->level(prev->level() + 1);
			}
		}
		
		return lines;
	}

	void BlockProcessor::make_list_hierarchy(shared_ptr<ListBlock> list)
	{
		int level = std::dynamic_pointer_cast<ListItemBlock>(list->children()[0])->level();
		int index = 0;

		vector<shared_ptr<Block>> result;
		vector<shared_ptr<Block>> children;

		for (int i = 1; (unsigned)i < list->children().size(); ++i)
		{
			shared_ptr<ListItemBlock> current = std::dynamic_pointer_cast<ListItemBlock>(list->children()[i]);

			if (current->level() > level)
			{
				children.push_back(current);
			}
			else if (current->level() == level)
			{
				if (children.size() > 0)
				{
					shared_ptr<ListBlock> child_list{ new ListBlock(list->buffer()) };
					child_list->start(children[0]->start()).end(children[children.size() - 1]->end());

					child_list->children(children);
					make_list_hierarchy(child_list);

					list->children()[index]->children().push_back(child_list);

					children.clear();
				}

				result.push_back(list->children()[index]);
				index = i;
			}
		}

		if (children.size() > 0)
		{
			shared_ptr<ListBlock> child_list{ new ListBlock(list->buffer()) };
			child_list->start(children[0]->start()).end(children[children.size() - 1]->end());

			child_list->children(children);
			make_list_hierarchy(child_list);

			list->children()[index]->children().push_back(child_list);
		}

		result.push_back(list->children()[index]);

		list->children(result);
	}
}