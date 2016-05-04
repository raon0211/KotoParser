#ifndef KOTOPARSER_H
#define KOTOPARSER_H

#include <string>
using std::wstring;

namespace kotoparser
{
	class KotoParser
	{
	public:
		wstring transform(wstring input);
	};
}

#endif