#include "block.h"
#include <string>
#include "utilities.h"

using std::string;

namespace kotoparser
{
	string Block::render()
	{
		return html_encode(content());
	}
}