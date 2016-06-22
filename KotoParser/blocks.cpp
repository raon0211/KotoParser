#include "blocks.h"
#include <string>
#include "utilities.h"

using std::wstring;

namespace kotoparser
{
	wstring HeadingBlock::render()
	{
		wstring result;

		result += L"<h" + std::to_wstring(_level) + L">";
		result += html_encode(content());
		result += L"</h" + std::to_wstring(_level) + L">";

		return result;
	}

	wstring BlockquoteBlock::render()
	{
		wstring result;

		result += L"<blockquote>";
		result += html_encode(content());
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
			result += html_encode(line->content());
			result += '\n';
		}
		
		result += L"</code>";
		result += L"</pre>\n";

		return result;
	}

	wstring TermBlock::render()
	{
		wstring result;

		result += L"<dt>";
		result += html_encode(content());
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

		result += L"<dd>";
		result += html_encode(content());
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

		result += L"<li>";

		result += html_encode(content());

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

		result += L"<p>";
		result += html_encode(content());
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