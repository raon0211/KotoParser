#include "block.h"
#include <string>

using std::wstring;

namespace kotoparser
{
	wstring Block::render()
	{
		return content();
	}
}