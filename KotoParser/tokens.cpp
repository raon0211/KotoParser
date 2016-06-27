#include "tokens.h"
#include "utilities.h"

namespace kotoparser
{
	wstring EmphasisToken::render()
	{
		wstring result;

		wstring name = is_strong() ? L"strong" : L"em";

		result += L"<" + name + L">";

		for (auto token : children())
		{
			result += token->render();
		}

		result += L"</" + name + L">";

		return result;
	}

	wstring EmphasisParseToken::render()
	{
		return html_encode(content());
	}

	wstring CodeToken::render()
	{
		wstring result;

		result += L"<code>";
		result += html_encode(content());
		result += L"</code>";

		return result;
	}

	wstring HtmlToken::render()
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

	wstring LinkToken::render()
	{
		wstring result;

		result += L"<a";

		result += L" href=\"";
		result += url();
		result += L"\"";

		result += L" class=\"";
		result += is_internal() ? L"internal" : L"external";
		result += L"\"";

		result += L">";

		result += html_encode(text());

		result += L"</a>";

		return result;
	}

	wstring ImageToken::render()
	{
		wstring result;

		result += L"<img";

		result += L" src=\"";
		result += url();
		result += L"\"";

		if (!is_null_or_whitespace(text()))
		{
			result += L" alt=\"";
			result += html_encode(text());
			result += L"\"";

			result += L" title=\"";
			result += html_encode(text());
			result += L"\"";
		}

		result += L" />";

		return result;
	}

	wstring TextToken::render()
	{
		return html_encode(content());
	}
}