#include "stdafx.h"
#include "CppUnitTest.h"
#include "kotoparser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace kotoparser;

namespace KotoParserTests
{		
	TEST_CLASS(KotoParser_Test)
	{
	public:
		
		TEST_METHOD(HeadingTest)
		{
			KotoParser parser;
			wstring result = parser.transform(L"# hello");

			Assert::AreEqual(result.c_str(), L"<h1>hello</h1>", L"Not same heading");
		}
	};
}