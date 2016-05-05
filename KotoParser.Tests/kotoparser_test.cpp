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

		TEST_METHOD(ListTest)
		{
			wstring result = kotoparser::transform(L"1. First\n * One\n * Two\n2. Second");

			Assert::AreEqual(L"<ol><li>First</li>\n</ol>\n<ul><li>One</li>\n<li>Two</li>\n</ul>\n<ol><li>Second</li>\n</ol>\n", result.c_str());
		}

		TEST_METHOD(DefinitionListTest)
		{
			wstring input =
L"; term : definition\n\
; term 2\n\
: definition 2";

			wstring output = kotoparser::transform(input);

			Assert::AreEqual(L"<dl><dt>term </dt>\n<dd>definition</dd>\n<dt>term 2</dt>\n<dd>definition 2</dd>\n</dl>\n", output.c_str());
		}
	};
}