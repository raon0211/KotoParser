#include "html_tag.h"
#include "utilities.h"

using std::pair;

namespace kotoparser
{
	vector<wstring> HtmlTag::allowed_tags = {
		L"b", L"blockquote", L"code", L"dd", L"dt", L"dl", L"del", L"em", L"h1", L"h2", L"h3", L"h4", L"h5", L"h6", L"i", L"kbd", L"li", L"ol", L"ul",
		L"p", L"pre", L"s", L"sub", L"sup", L"strong", L"strike", L"img", L"a"
	};

	map<wstring, vector<wstring>> HtmlTag::allowed_attribute_map = {
		{ L"a", { L"href", L"title", L"class" } },
		{ L"img", { L"src", L"width", L"height", L"alt", L"title", L"class" } },
		{ L"div", { L"class" } }
	};

	map<wstring, vector<HtmlTagType>> HtmlTag::tag_types_map = {
		{ L"p", { HtmlTagType::Block } },
		{ L"div", { HtmlTagType::Block } },
		{ L"h1", { HtmlTagType::Block } },
		{ L"h2", { HtmlTagType::Block } },
		{ L"h3", { HtmlTagType::Block } },
		{ L"h4", { HtmlTagType::Block } },
		{ L"h5", { HtmlTagType::Block } },
		{ L"h6", { HtmlTagType::Block } },
		{ L"blockquote", { HtmlTagType::Block } },
		{ L"pre", { HtmlTagType::Block } },
		{ L"table", { HtmlTagType::Block } },
		{ L"dl", { HtmlTagType::Block } },
		{ L"ol", { HtmlTagType::Block } },
		{ L"ul", { HtmlTagType::Block } },
		{ L"li", { HtmlTagType::Block } },
		{ L"dd", { HtmlTagType::Block } },
		{ L"dt", { HtmlTagType::Block } },
		{ L"td", { HtmlTagType::Block } },
		{ L"th", { HtmlTagType::Block } },
		{ L"form", { HtmlTagType::Block } },
		{ L"fieldset", { HtmlTagType::Block } },
		{ L"legend", { HtmlTagType::Block } },
		{ L"address", { HtmlTagType::Block } },
		{ L"hr", { HtmlTagType::Block, HtmlTagType::NoClosing } },
		{ L"br", { HtmlTagType::NoClosing } },
		{ L"math", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ L"ins", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ L"del", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ L"img", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ L"del", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ L"!", { HtmlTagType::Block } }
	};

	bool HtmlTag::safe()
	{
		wstring lower_name = _wcslwr(&name()[0]);
		
		if (std::find(allowed_tags.begin(), allowed_tags.end(), lower_name) == allowed_tags.end())
		{
			return false;
		}

		// If no attributes are allowed,
		if (allowed_attribute_map.find(_wcslwr(&name()[0])) == allowed_attribute_map.end())
		{
			// Having no attributes is safe
			return attributes().size() == 0;
		}

		for (pair<wstring, wstring> attribute : attributes())
		{
			if (allowed_attribute_map.find(_wcslwr(&attribute.first[0])) == allowed_attribute_map.end())
			{
				return false;
			}
		}

		if (attributes().find(L"href") != attributes().end())
		{
			if (!is_safe_url(attributes().at(L"href")))
			{
				return false;
			}
		}

		if (attributes().find(L"src") != attributes().end())
		{
			if (!is_safe_url(attributes().at(L"src")))
			{
				return false;
			}
		}

		return true;
	}

	vector<HtmlTagType> HtmlTag::types()
	{
		if (tag_types_map.find(_wcslwr(&name()[0])) == tag_types_map.end())
		{
			vector<HtmlTagType> types = { HtmlTagType::Inline };
			return types;
		}

		return tag_types_map.at(name());
	}

	bool HtmlTag::is_safe_url(wstring url)
	{
		return str_starts_with(url, L"http://") ||
			   str_starts_with(url, L"https://") ||
			   str_starts_with(url, L"ftp://");
	}
}