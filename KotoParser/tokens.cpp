#include "tokens.h"
#include "utilities.h"

namespace kotoparser
{
	string EmphasisToken::render()
	{
		string result;

		string name = is_strong() ? "strong" : "em";

		result += "<" + name + ">";

		for (auto token : children())
		{
			result += token->render();
		}

		result += "</" + name + ">";

		return result;
	}

	string EmphasisParseToken::render()
	{
		return html_encode(content());
	}

	string CodeToken::render()
	{
		string result;

		result += "<code>";
		result += html_encode(content());
		result += "</code>";

		return result;
	}

	string HtmlToken::render()
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

	string LinkToken::render()
	{
		string result;

		result += "<a";

		result += " href=\"";
		result += url();
		result += "\"";

		result += " class=\"";
		result += is_internal() ? "internal" : "external";
		result += "\"";

		result += ">";

		result += html_encode(text());

		result += "</a>";

		return result;
	}

	string ImageToken::render()
	{
		string result;

		result += "<img";

		result += " src=\"";
		result += url();
		result += "\"";

		if (!is_null_or_whitespace(text()))
		{
			result += " alt=\"";
			result += html_encode(text());
			result += "\"";

			result += " title=\"";
			result += html_encode(text());
			result += "\"";
		}

		result += " />";

		return result;
	}

	string TextToken::render()
	{
		return html_encode(content());
	}
}