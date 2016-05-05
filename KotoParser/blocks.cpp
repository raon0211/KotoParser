#include "blocks.h"
#include <string>

using std::wstring;

namespace kotoparser
{
	wstring HeadingBlock::render()
	{
		wstring result;

		result += L"<h" + std::to_wstring(_level) + L">";
		result += content();
		result += L"</h" + std::to_wstring(_level) + L">";

		return result;
	}

	wstring BlockquoteBlock::render()
	{
		wstring result;

		result += L"<blockquote>";
		result += content();
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
			result += line->content();
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
		result += content();
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
		result += content();
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

		result += content();

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
		result += content();
		result += L"</p>";

		result += L"\n";

		return result;
	}

	wstring PlainBlock::render()
	{
		return content();
	}

	wstring BlankBlock::render()
	{
		return L"";
	}
}