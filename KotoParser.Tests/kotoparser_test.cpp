#include "stdafx.h"
#include "CppUnitTest.h"
#include "kotoparser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KotoParserTests
{		
	TEST_CLASS(KotoParser_Test)
	{
	public:
		
		TEST_METHOD(HeadingTest)
		{
			wstring result = kotoparser::transform(L"# hello");

			Assert::AreEqual(L"<h1>hello</h1>", result.c_str());
		}

		TEST_METHOD(BlockquoteTest)
		{
			wstring result = kotoparser::transform(L"> Blockquote");
			
			Assert::AreEqual(L"<blockquote>Blockquote</blockquote>", result.c_str());
		}

		TEST_METHOD(CodeTest)
		{
			wstring result = kotoparser::transform(L"```cpp\nhello\n```");

			Assert::AreEqual(L"<pre><code class=\"cpp\">hello\n</code></pre>\n", result.c_str());
		}
	};
}