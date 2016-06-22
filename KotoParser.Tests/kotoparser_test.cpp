#include "stdafx.h"
#include "CppUnitTest.h"
#include "kotoparser.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

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
			wstring input =
L"1. First\n\
 * One\n\
 * Two\n\
2. Second";

			wstring result = kotoparser::transform(input);

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

		TEST_METHOD(HtmlTest)
		{
			map<wstring, wstring> dic
			{
				{
					L"<div class=\"wrap\">aa<div>aa</div></div>", L"<div class=\"wrap\">aa<div>aa</div></div>"
				},
				{ L"<h1><h2>dsdsds</h1></h2>", L"<h1><p>&lt;h2&gt;dsdsds</p>\n</h1>\n<p>&lt;/h2&gt;</p>\n" }
			};

			for (auto item : dic)
			{
				wstring output = kotoparser::transform(item.first);

				Assert::AreEqual(item.second, output);
			}
		}
	};
}