#ifndef TABLE_PROCESSOR_H
#define TABLE_PROCESSOR_H

#include <vector>
#include <memory>
#include "string_scanner.h"
#include "block.h"
#include "blocks.h"

using std::vector;
using std::shared_ptr;

namespace kotoparser
{
	class TableProcessor : public StringScanner
	{
	public:
		TableProcessor(string input) : StringScanner(input)
		{
		}
		TableProcessor(string input, int start, int length) : StringScanner(input, start, length)
		{
		}

		vector<shared_ptr<Block>> process();

	private:
		bool is_table_separator(char c);
	};
}
#endif