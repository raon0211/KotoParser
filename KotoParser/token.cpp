#include "token.h"
#include "utilities.h"

namespace kotoparser
{
	string Token::render()
	{
		return html_encode(content());
	}
}