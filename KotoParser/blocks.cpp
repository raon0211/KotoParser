#include "blocks.h"
#include <string>
#include "utilities.h"
#include "inline_processor.h"

using std::string;

namespace kotoparser
{
	string HeadingBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<h" + std::to_string(_level) + ">";
		result += processor.transform();
		result += "</h" + std::to_string(_level) + ">";

		return result;
	}

	string BlockquoteBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<blockquote>";
		result += processor.transform();
		result += "</blockquote>";

		return result;
	}

	string CodeBlock::render()
	{
		string result;

		result += "<pre>";
		result += "<code";
		if (language().length() != 0)
		{
			result += " class=\"" + language() + "\"";
		}
		result += ">";

		for (auto line : children())
		{
			InlineProcessor processor(line->content());

			result += processor.transform();
			result += '\n';
		}
		
		result += "</code>";
		result += "</pre>\n";

		return result;
	}

	string TermBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<dt>";
		result += processor.transform();
		result += "</dt>";

		result += "\n";

		return result;
	}

	string DefinitionListBlock::render()
	{
		string result;

		result += "<dl>";

		for (auto child : children())
		{
			result += child->render();
		}

		result += "</dl>";
		result += "\n";

		return result;
	}

	string DefinitionBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<dd>";
		result += processor.transform();
		result += "</dd>";

		result += "\n";

		return result;
	}

	string TableBlock::render()
	{
		string result;
		
		result += "<table>";
		
		for (auto row : children())
		{
			result += row->render();
		}

		result += "</table>";
		result += "\n";

		return result;
	}

	string TableRowBlock::render()
	{
		string result;

		result += "<tr>";

		for (auto row : children())
		{
			result += row->render();
		}

		result += "</tr>";
		result += "\n";

		return result;
	}

	string TableDataBlock::render()
	{
		string result;
		string name = is_header() ? "th" : "td";
		InlineProcessor processor(content());

		result += "<";
		result += name;
		
		result += " class=\"";
		switch (alignment())
		{
			case TableDataAlignment::Left:
				result += "left";
				break;
			case TableDataAlignment::Center:
				result += "center";
				break;
			case TableDataAlignment::Right:
				result += "right";
				break;
		}
		result += "\"";

		if (colspan() > 1)
		{
			result += " colspan=\"";
			result += std::to_string(colspan());
			result += "\"";
		}

		if (rowspan() > 1)
		{
			result += " rowspan=\"";
			result += std::to_string(rowspan());
			result += "\"";
		}

		result += ">";

		result += processor.transform();

		result += "</" + name + ">";
		result += "\n";

		return result;
	}

	string ListBlock::render()
	{
		string result;

		if (children().size() > 0)
		{
			string name;
			
			if ((std::dynamic_pointer_cast<ListItemBlock>(children()[0])->is_ordered()) == true)
			{
				name = "ol";
			}
			else
			{
				name = "ul";
			}

			result += "<" + name + ">";

			for (auto child : children())
			{
				result += child->render();
			}

			result += "</" + name + ">";

			result += "\n";
		}

		return result;
	}

	string ListItemBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<li>";

		result += processor.transform();

		for (auto child : children())
		{
			result += child->render();
		}

		result += "</li>";

		result += "\n";

		return result;
	}

	string ParagraphBlock::render()
	{
		string result;
		InlineProcessor processor(content());

		result += "<p>";
		result += processor.transform();
		result += "</p>";

		result += "\n";

		return result;
	}

	string HtmlBlock::render()
	{
		string result;

		string name = _strlwr(&tag()->name()[0]);

		result += "<";
		result += name;

		for (auto attribute : tag()->attributes())
		{
			result += " ";
			result += attribute.first;
			result += "=";
			result += '"';
			result += attribute.second;
			result += '"';
		}

		if (tag()->self_closed() ||
			contains(tag()->types(), HtmlTagType::NoClosing))
		{
			result += " />";
		}
		else
		{
			result += ">";

			for (auto child : children())
			{
				result += child->render();
			}

			result += "</";
			result += name;
			result += ">";
		}

		result += "\n";

		return result;
	}

	string PlainBlock::render()
	{
		return html_encode(content());
	}

	string BlankBlock::render()
	{
		return "";
	}
}