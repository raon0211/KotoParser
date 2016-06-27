#ifndef HTML_TAG_H
#define HTML_TAG_H

#include <map>
#include <string>
#include <vector>

using std::map;
using std::wstring;
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
		wstring name() const { return _name; }
		HtmlTag& name(wstring new_name) { _name = new_name; return *this; }

		map<wstring, wstring>& attributes() { return _attributes; }
		HtmlTag& attributes(map<wstring, wstring> new_attributes) { _attributes = new_attributes; return *this; }

		bool self_closed() const { return _self_closed; }
		HtmlTag& self_closed(bool new_self_closed) { _self_closed = new_self_closed; return *this; }

		bool closing() const { return _closing; }
		HtmlTag& closing(bool new_closing) { _closing = new_closing; return *this; }

		vector<HtmlTagType> types();
		bool safe();

		HtmlTag(wstring name)
		{
			this->name(name);
		}

	private:
		wstring _name;
		map<wstring, wstring> _attributes;
		bool _self_closed;
		bool _closing;
		static vector<wstring> allowed_tags;
		static map<wstring, vector<wstring>> allowed_attribute_map;
		static map<wstring, vector<HtmlTagType>> tag_types_map;
	};
}

#endif