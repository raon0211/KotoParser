#ifndef HTML_TAG_H
#define HTML_TAG_H

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

namespace kotoparser
{
	enum class HtmlTagType
	{
		Block,
		Inline,
		NoClosing
	};

	class HtmlTag
	{
	public:
		string name() const { return _name; }
		HtmlTag& name(string new_name) { _name = new_name; return *this; }

		map<string, string>& attributes() { return _attributes; }
		HtmlTag& attributes(map<string, string> new_attributes) { _attributes = new_attributes; return *this; }

		bool self_closed() const { return _self_closed; }
		HtmlTag& self_closed(bool new_self_closed) { _self_closed = new_self_closed; return *this; }

		bool closing() const { return _closing; }
		HtmlTag& closing(bool new_closing) { _closing = new_closing; return *this; }

		vector<HtmlTagType> types();
		bool safe();

		HtmlTag(string name)
		{
			this->name(name);
		}

	private:
		string _name;
		map<string, string> _attributes;
		bool _self_closed;
		bool _closing;
		static vector<string> allowed_tags;
		static map<string, vector<string>> allowed_attribute_map;
		static map<string, vector<HtmlTagType>> tag_types_map;
	};
}

#endif