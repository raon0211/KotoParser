#include "token.h"
#include "utilities.h"

namespace kotoparser
{
	wstring Token::render()
	{
		return html_encode(content());
	}
}