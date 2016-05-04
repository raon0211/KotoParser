#include "block.h"

namespace kotoparser
{
	class HeadingBlock : public Block
	{
	public:
		int level() const { return _level; }
		HeadingBlock& level(int new_level) { _level = new_level; return *this; }

		HeadingBlock(wstring buffer, int level) : Block(buffer)
		{
			_type = block_type::heading;
			_level = level;
		}

		virtual wstring render()
		{
			wstring result;

			result += L"<h" + std::to_wstring(_level) + L">";
			result += content();
			result += L"</h" + std::to_wstring(_level) + L">";

			return result;
		}
	private:
		int _level;
	};
}