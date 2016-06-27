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
					L"<div class=\"wrap\">aa<div>aa</div></div>", L"<div class=\"wrap\"><p>aa<div>aa</div>\n</p>\n</div>\n"
				},
				{ L"<h1><h2>dsdsds</h1></h2>", L"<h1><p>&lt;h2&gt;dsdsds</p>\n</h1>\n<p>&lt;/h2&gt;</p>\n" }
			};

			for (auto item : dic)
			{
				wstring output = kotoparser::transform(item.first);

				Assert::AreEqual(item.second, output);
			}
		}

		TEST_METHOD(StrongTest)
		{
			wstring input =
L"**strong**\n\
*emphasized*\n\
\n\
***strong and emphasized** emphasized*\n\
\n\
***dsds* d**8 dsd**d**";

			wstring output = kotoparser::transform(input);

			Assert::AreEqual(L"<p><strong>strong</strong>\n<em>emphasized</em></p>\n<p><em><strong>strong and emphasized</strong> emphasized</em></p>\n<p><strong><em>dsds</em> d</strong>8 dsd<strong>d</strong></p>\n", output.c_str());
		}

		TEST_METHOD(CompositeTest)
		{
			wstring input = L"## Header\n\n----------\n\nSome **bold** Some *italic* and [a link][1] \n\nA little code sample\n\n    </head>\n    <title>Web Page Title</title>\n    </head>\n\nA picture\n\n![alt text][2]\n\nA list\n\n- apples\n- oranges\n- eggs\n\nA numbered list\n\n1. a\n2. b\n3. c\n\nA little quote\n\n> It is now time for all good men to come to the aid of their country. \n\nA final paragraph.\n\n  [1]: http://www.google.com\n  [2]: http://www.google.com/intl/en_ALL/images/logo.gif";

			wstring output = kotoparser::transform(input);

			Assert::AreEqual(L"<div class=\"wrap\"><h1>Heading</h1>\n<blockquote>Blockquote</blockquote>\n</div>\n<pre><code class=\"language-c-sharp\">CODE\n</code></pre>\n<hr />\n<p><img src=\"http://google.com/\" /></p>\n<p>&lt;span&gt;**paragraph**<br />\n<a href=\"이건 좀 아니지 않은지\" class=\"internal\">이건 좀 아니지 않은지</a>\r\n<span class=\"highlight\">testing..</span>\n</p>\n<dl><dt>term </dt>\n<dd>definition</dd>\n<dt>term 2</dt>\n<dd>definition 2</dd>\n</dl>\n<p>`<code>Lorem ipsum</code> <a href=\"대문\" class=\"internal\">이곳</a>으로 이동하세요.</p>\n<p>%test</p>\n<ol><li>You must do this.<ol><li>You also have to do this.</li>\n<li>Do you understand?</li>\n</ol>\n</li>\n<li>Yup, I understand.</li>\n</ol>\n<ul><li>Does this work too?<ul><li>Yes, it works.\r\nparagraph</li>\n</ul>\n</li>\n</ul>\n", output.c_str());
		}
	};
}