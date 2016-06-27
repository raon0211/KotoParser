#include "blocks.h"
#include <string>
#include "utilities.h"
#include "inline_processor.h"

using std::wstring;

namespace kotoparser
{
	wstring HeadingBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<h" + std::to_wstring(_level) + L">";
		result += processor.transform();
		result += L"</h" + std::to_wstring(_level) + L">";

		return result;
	}

	wstring BlockquoteBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<blockquote>";
		result += processor.transform();
		result += L"</blockquote>";

		return result;
	}

	wstring CodeBlock::render()
	{
		wstring result;

		result += L"<pre>";
		result += L"<code";
		if (language().length() != 0)
		{
			result += L" class=\"" + language() + L"\"";
		}
		result += L">";

		for (auto line : children())
		{
			InlineProcessor processor(line->content());

			result += processor.transform();
			result += '\n';
		}
		
		result += L"</code>";
		result += L"</pre>\n";

		return result;
	}

	wstring TermBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<dt>";
		result += processor.transform();
		result += L"</dt>";

		result += L"\n";

		return result;
	}

	wstring DefinitionListBlock::render()
	{
		wstring result;

		result += L"<dl>";

		for (auto child : children())
		{
			result += child->render();
		}

		result += L"</dl>";
		result += L"\n";

		return result;
	}

	wstring DefinitionBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<dd>";
		result += processor.transform();
		result += L"</dd>";

		result += L"\n";

		return result;
	}

	wstring ListBlock::render()
	{
		wstring result;

		if (children().size() > 0)
		{
			wstring name;
			
			if ((std::dynamic_pointer_cast<ListItemBlock>(children()[0])->is_ordered()) == true)
			{
				name = L"ol";
			}
			else
			{
				name = L"ul";
			}

			result += L"<" + name + L">";

			for (auto child : children())
			{
				result += child->render();
			}

			result += L"</" + name + L">";

			result += L"\n";
		}

		return result;
	}

	wstring ListItemBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<li>";

		result += processor.transform();

		for (auto child : children())
		{
			result += child->render();
		}

		result += L"</li>";

		result += L"\n";

		return result;
	}

	wstring ParagraphBlock::render()
	{
		wstring result;
		InlineProcessor processor(content());

		result += L"<p>";
		result += processor.transform();
		result += L"</p>";

		result += L"\n";

		return result;
	}

	wstring HtmlBlock::render()
	{
		wstring result;

		wstring name = _wcslwr(&tag()->name()[0]);

		result += L"<";
		result += name;

		for (auto attribute : tag()->attributes())
		{
			result += L" ";
			result += attribute.first;
			result += L"=";
			result += '"';
			result += attribute.second;
			result += '"';
		}

		if (tag()->self_closed() ||
			contains(tag()->types(), HtmlTagType::NoClosing))
		{
			result += L" />";
		}
		else
		{
			result += L">";

			for (auto child : children())
			{
				result += child->render();
			}

			result += L"</";
			result += name;
			result += L">";
		}

		result += L"\n";

		return result;
	}

	wstring PlainBlock::render()
	{
		return html_encode(content());
	}

	wstring BlankBlock::render()
	{
		return L"";
	}
}