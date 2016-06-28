#include "block_processor.h"
#include <vector>
#include <string>
#include <memory>
#include "blocks.h"
#include "utilities.h"
#include "table_processor.h"

using std::reference_wrapper;
using std::vector;
using std::string;
using std::shared_ptr;

namespace kotoparser
{
	vector<shared_ptr<Block>> BlockProcessor::process()
	{
		// First, parse all blocks according to the line
		vector<shared_ptr<Block>> blocks = parse();

		// Now, the blocks are parsed by lines, so
		// Transform Paragraph - Paragraph - Blank - Paragraph - ListItem - ListItem - Heading
		// into Paragraph - Paragraph - List - Heading
		blocks = integrate(blocks);

		return blocks;
	}

	vector<shared_ptr<Block>> BlockProcessor::parse()
	{
		vector<shared_ptr<Block>> blocks;

		while (!at_file_end())
		{
			// We're starting at the first character of line
			// or, in the case of definition lists, ':'
			int line_start = position;
			int indent = 0;

			BlockType last_block_type = blocks.size() > 0 ? blocks[blocks.size() - 1]->type() : BlockType::Blank;
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
			// ; Definition : List
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
			// <html>
			else if (current() == '<')
			{
				block.reset(new HtmlBlock(input));
				block->start(position);

				if (!make_html(std::dynamic_pointer_cast<HtmlBlock>(block)))
				{
					block = NULL;
					position = line_start;
				}
			}
			else if (current() == '|' || current() == '^')
			{
				block.reset(new TableRowBlock(input));
				block->start(position);

				skip_to_line_end();

				if (char_at_offset(-1) == '|' ||
					char_at_offset(-1) == '^')
				{
					block->end(position);

					string row_content = block->content();
					// remove the trailing '^' or '|'
					TableProcessor processor(row_content, 0, row_content.length() - 1);

					block->children(processor.process());
				}
				else
				{
					block = NULL;
					position = line_start;
				}
			}
			else if (indent = skip_linespace(),
					 current() == ':' && (last_block_type == BlockType::Term || last_block_type == BlockType::Definition))
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
			// * Unordered List
			else if ((current() == '*' || current() == '-' || current() == '+') && is_linespace(char_at_offset(1)) && char_is_first_of_line())
			{
				skip_forward(1);
				skip_linespace();

				block.reset(new ListItemBlock(input, false, check_indent()));
				block->start(position);

				skip_to_line_end();

				block->end(position);
			}
			// 1. Ordered List
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
			// Blank line
			else if (at_line_end())
			{
				block.reset(new BlankBlock(input));
				block->start(position);
				block->end(position);
			}
		
			if (block == NULL)
			{
				block.reset(new ParagraphBlock(input));
				block->start(position);
				skip_to_line_end();
				block->end(position);
			}
			
			blocks.push_back(block);

			skip_line_end();
		}

		return blocks;
	}

	bool BlockProcessor::make_code_block(shared_ptr<CodeBlock> block)
	{
		char delim = current();

		// Check if there are three `s
		mark();

		while (current() == delim)
		{
			skip_forward(1);
		}

		string fence = extract();

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
			string language = extract();

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

		// Remove unnecessary line ends
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

	bool BlockProcessor::make_html(shared_ptr<HtmlBlock> block)
	{
		int start = position;

		shared_ptr<HtmlTag> opening_tag = parse_tag();

		if (opening_tag == NULL)
		{
			return false;
		}

		int content_start = position;

		// Closing tag came first, wrong
		if (opening_tag->closing())
		{
			return false;
		}

		// Is the opening tag safe?
		bool safe = opening_tag->safe();

		vector<HtmlTagType> types = opening_tag->types();

		// If it is not a block element, it is not necessary to parse it
		if (!contains(types, HtmlTagType::Block))
		{
			return false;
		}

		if (contains(types, HtmlTagType::NoClosing) ||
			opening_tag->self_closed())
		{
			skip_linespace();
			skip_line_end();

			block->tag(opening_tag)
				.end(position)
				.type(safe ? BlockType::Html : BlockType::Plain);

			return true;
		}

		if (contains(types, HtmlTagType::Inline))
		{
			skip_linespace();
			if (!at_line_end())
			{
				return false;
			}
		}

		vector<shared_ptr<Block>> child_blocks;

		int depth = 1;

		while (!at_file_end())
		{
			// Failed to find <, stop
			if (!find('<'))
			{
				break;
			}

			int current_tag_start = position;

			shared_ptr<HtmlTag> tag = parse_tag();
			if (tag == NULL)
			{
				skip_forward(1);
				continue;
			}

			if (tag->self_closed())
			{
				continue;
			}

			if (tag->name() == opening_tag->name())
			{
				if (tag->closing())
				{
					depth--;

					if (depth == 0)
					{
						skip_linespace();
						
						block->tag(opening_tag);
						block->end(position);

						BlockProcessor processor(input, content_start, current_tag_start - content_start);
						block->children(processor.process());

						return true;
					}
				}
				else
				{
					depth++;
				}
			}
		}

		return false;
	}

	shared_ptr<HtmlTag> BlockProcessor::parse_tag()
	{
		int start = position;

		if (current() != '<')
		{
			return NULL;
		}

		// skip '<'
		skip_forward(1);

		if (does_match("!--"))
		{
			skip_forward(3);

			mark();

			if (find("-->"))
			{
				HtmlTag t("!");
				t.attributes()["content"] = extract();
				t.self_closed(true);

				skip_forward(3);
			}
		}

		bool closing = false;

		if (current() == '/')
		{
			closing = true;
			skip_forward(1);
		}

		string name = extract_word_block();
		if (is_null_or_whitespace(name))
		{
			return NULL;
		}

		shared_ptr<HtmlTag> tag{ new HtmlTag(name) };
		tag->closing(closing);

		if (closing)
		{
			// There is an attribute in a closing tag!
			if (current() != '>')
			{
				return NULL;
			}

			skip_forward(1);
			return tag;
		}

		while (!at_file_end())
		{
			skip_whitespace();

			if (does_match("/>"))
			{
				tag->self_closed(true);
				skip_forward(2);
				return tag;
			}

			if (does_match(">"))
			{
				skip_forward(1);
				return tag;
			}

			string attribute_name = extract_word_block();
			if (is_null_or_whitespace(attribute_name))
			{
				return NULL;
			}

			skip_whitespace();

			// check if the attribute has value, like src=""
			// or doesn't have a value, like autofocus
			bool has_value = does_match('=');

			if (has_value)
			{
				skip_forward(1);
				skip_whitespace();

				bool has_quote = does_match('"') || does_match("'");

				if (has_quote)
				{
					char quote = current();

					skip_forward(1);
					mark();

					if (!find(quote))
					{
						return NULL;
					}

					tag->attributes()[attribute_name] = extract();

					skip_forward(1);
				}
				else
				{
					mark();

					while (!at_file_end() &&
						!(is_linespace(current()) || current() == '>' || current() == '/'))
					{
						skip_forward(1);
					}

					if (!at_file_end())
					{
						tag->attributes()[attribute_name] = extract();
					}
				}
			}
			else
			{
				tag->attributes()[attribute_name] = "";
			}
		}

		return NULL;
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
			BlockType lines_type = lines.size() > 0 ? lines[0]->type() : BlockType::Blank;

			switch (block->type())
			{
				case BlockType::Blank:
					add_integrated_block(result, lines);
					break;

				case BlockType::Paragraph:
					if (lines_type == BlockType::Blank ||
						lines_type == BlockType::Paragraph ||
						lines_type == BlockType::Blockquote ||
						lines_type == BlockType::ListItem)
					{
						lines.push_back(block);
						break;
					}

					add_integrated_block(result, lines);
					lines.push_back(block);
					break;

				case BlockType::TableRow:
					if (lines_type == BlockType::TableRow)
					{
						lines.push_back(block);
						break;
					}

					add_integrated_block(result, lines);
					lines.push_back(block);
					break;

				case BlockType::ListItem:
					if (lines_type == BlockType::Blank)
					{
						lines.push_back(block);
						break;
					}
					else if (lines_type == BlockType::ListItem)
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

				case BlockType::Term:
				case BlockType::Definition:
					if (lines_type == BlockType::Blank ||
						lines_type == BlockType::Term ||
						lines_type == BlockType::Definition)
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
			case BlockType::Paragraph:
			{
				shared_ptr<ParagraphBlock> p{ new ParagraphBlock(lines[0]->buffer()) };
				p->start(lines[0]->start()).end(lines[lines.size() - 1]->end());

				lines.clear();

				return p;
			}

			case BlockType::TableRow:
			{
				shared_ptr<TableBlock> table{ new TableBlock(lines[0]->buffer()) };
				table->start(lines[0]->start()).end(lines[lines.size() - 1]->end());
				table->children(lines);

				lines.clear();

				return table;
			}

			case BlockType::ListItem:
			{
				shared_ptr<ListBlock> list{ new ListBlock(lines[0]->buffer()) };
				list->start(lines[0]->start()).end(lines[lines.size() - 1]->end());
				list->children(set_list_level(lines));

				make_list_hierarchy(list);

				lines.clear();

				return list;
			}

			case BlockType::Term:
			case BlockType::Definition:
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
			if (lines[i]->type() == BlockType::Paragraph)
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
					child_list
						->start(children[0]->start())
						.end(children[children.size() - 1]->end());

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