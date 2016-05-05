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

	wstring PlainBlock::render()
	{
		return content();
	}

	wstring BlankBlock::render()
	{
		return L"";
	}
}