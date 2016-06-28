#include "html_tag.h"
#include "utilities.h"

using std::pair;

namespace kotoparser
{
	vector<string> HtmlTag::allowed_tags = {
		"b", "blockquote", "code", "dd", "dt", "d", "de", "em", "h1", "h2", "h3", "h4", "h5", "h6", "i", "kbd", "li", "o", "u",
		"p", "pre", "s", "sub", "sup", "strong", "strike", "img", "a"
	};

	map<string, vector<string>> HtmlTag::allowed_attribute_map = {
		{ "a", { "href", "title", "class" } },
		{ "img", { "src", "width", "height", "alt", "title", "class" } },
		{ "div", { "class" } }
	};

	map<string, vector<HtmlTagType>> HtmlTag::tag_types_map = {
		{ "p", { HtmlTagType::Block } },
		{ "div", { HtmlTagType::Block } },
		{ "h1", { HtmlTagType::Block } },
		{ "h2", { HtmlTagType::Block } },
		{ "h3", { HtmlTagType::Block } },
		{ "h4", { HtmlTagType::Block } },
		{ "h5", { HtmlTagType::Block } },
		{ "h6", { HtmlTagType::Block } },
		{ "blockquote", { HtmlTagType::Block } },
		{ "pre", { HtmlTagType::Block } },
		{ "table", { HtmlTagType::Block } },
		{ "d", { HtmlTagType::Block } },
		{ "o", { HtmlTagType::Block } },
		{ "u", { HtmlTagType::Block } },
		{ "li", { HtmlTagType::Block } },
		{ "dd", { HtmlTagType::Block } },
		{ "dt", { HtmlTagType::Block } },
		{ "td", { HtmlTagType::Block } },
		{ "th", { HtmlTagType::Block } },
		{ "form", { HtmlTagType::Block } },
		{ "fieldset", { HtmlTagType::Block } },
		{ "legend", { HtmlTagType::Block } },
		{ "address", { HtmlTagType::Block } },
		{ "hr", { HtmlTagType::Block, HtmlTagType::NoClosing } },
		{ "br", { HtmlTagType::NoClosing } },
		{ "math", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ "ins", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ "de", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ "img", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ "de", { HtmlTagType::Block, HtmlTagType::Inline } },
		{ "!", { HtmlTagType::Block } }
	};

	bool HtmlTag::safe()
	{
		string lower_name = _strlwr(&name()[0]);
		
		if (!contains(keys(tag_types_map), lower_name))
		{
			return false;
		}

		// If no attributes are allowed,
		if (allowed_attribute_map.find(_strlwr(&name()[0])) == allowed_attribute_map.end())
		{
			// Having no attributes is safe
			return attributes().size() == 0;
		}

		for (pair<string, string> attribute : attributes())
		{
			if (allowed_attribute_map.find(_strlwr(&attribute.first[0])) == allowed_attribute_map.end())
			{
				return false;
			}
		}

		if (attributes().find("href") != attributes().end())
		{
			if (!is_safe_url(attributes().at("href")))
			{
				return false;
			}
		}

		if (attributes().find("src") != attributes().end())
		{
			if (!is_safe_url(attributes().at("src")))
			{
				return false;
			}
		}

		return true;
	}

	vector<HtmlTagType> HtmlTag::types()
	{
		if (tag_types_map.find(_strlwr(&name()[0])) == tag_types_map.end())
		{
			vector<HtmlTagType> types = { HtmlTagType::Inline };
			return types;
		}

		return tag_types_map.at(name());
	}
}