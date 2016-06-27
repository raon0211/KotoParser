#include "block.h"
#include <string>
#include "utilities.h"

using std::wstring;

namespace kotoparser
{
	wstring Block::render()
	{
		return html_encode(content());
	}
}