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
			string result = kotoparser::transform("# hello");

			Assert::AreEqual("<h1>hello</h1>", result.c_str());
		}

		TEST_METHOD(BlockquoteTest)
		{
			string result = kotoparser::transform("> Blockquote");
			
			Assert::AreEqual("<blockquote>Blockquote</blockquote>", result.c_str());
		}

		TEST_METHOD(CodeTest)
		{
			string result = kotoparser::transform("```cpp\nhello\n```");

			Assert::AreEqual("<pre><code class=\"cpp\">hello\n</code></pre>\n", result.c_str());
		}

		TEST_METHOD(ListTest)
		{
			string input =
"1. First\n\
 * One\n\
 * Two\n\
2. Second";

			string result = kotoparser::transform(input);

			Assert::AreEqual("<ol><li>First</li>\n</ol>\n<ul><li>One</li>\n<li>Two</li>\n</ul>\n<ol><li>Second</li>\n</ol>\n", result.c_str());
		}

		TEST_METHOD(DefinitionListTest)
		{
			string input =
				"; term : definition\n\
; term 2\n\
: definition 2";

			string output = kotoparser::transform(input);

			Assert::AreEqual("<dl><dt>term </dt>\n<dd>definition</dd>\n<dt>term 2</dt>\n<dd>definition 2</dd>\n</dl>\n", output.c_str());
		}

		TEST_METHOD(HtmlTest)
		{
			map<string, string> dic
			{
				{
					"<div class=\"wrap\">aa<div>aa</div></div>", "<div class=\"wrap\"><p>aa<div>aa</div>\n</p>\n</div>\n"
				},
				{ "<h1><h2>dsdsds</h1></h2>", "<h1><p>&lt;h2&gt;dsdsds</p>\n</h1>\n<p>&lt;/h2&gt;</p>\n" }
			};

			for (auto item : dic)
			{
				string output = kotoparser::transform(item.first);

				Assert::AreEqual(item.second, output);
			}
		}

		TEST_METHOD(TableTest)
		{
			string input =
"^  Hello  ^  Hello  ^^\n\
| Hello || Hello |";

			string output = kotoparser::transform(input);

			Assert::AreEqual("<table><tr><th class=\"center\">Hello</th>\n<th class=\"center\" colspan=\"2\">Hello</th>\n</tr>\n<tr><td class=\"left\" colspan=\"2\">Hello</td>\n<td class=\"left\">Hello</td>\n</tr>\n</table>\n", output.c_str());
		}

		TEST_METHOD(StrongTest)
		{
			string input =
"**strong**\n\
*emphasized*\n\
\n\
***strong and emphasized** emphasized*\n\
\n\
***dsds* d**8 dsd**d**";

			string output = kotoparser::transform(input);

			Assert::AreEqual("<p><strong>strong</strong>\n<em>emphasized</em></p>\n<p><em><strong>strong and emphasized</strong> emphasized</em></p>\n<p><strong><em>dsds</em> d</strong>8 dsd<strong>d</strong></p>\n", output.c_str());
		}

		TEST_METHOD(CompositeTest)
		{
			string input = "## Header\n\nSome **bold** Some *italic* and [a link](a link) \n\nA little code sample\n\n```\n</head>\n<title>Web Page Title</title>\n</head>\n```\n\nA picture\n\n![alt text](http://google.com)\n\nA list\n\n* apples\n* oranges\n* eggs\n\nA numbered list\n\n1. a\n2. b\n3. c\n\nA little quote\n\n> It is now time for all good men to come to the aid of their country. \n\nA final paragraph.";

			string output = kotoparser::transform(input);

			Assert::AreEqual("<h2>Header</h2><p>Some <strong>bold</strong> Some <em>italic</em> and <a href=\"a link\" class=\"external\">a link</a> </p>\n<p>A little code sample</p>\n<pre><code>&lt;/head&gt;\n<title>Web Page Title</title>\n\n&lt;/head&gt;\n</code></pre>\n<p>A picture</p>\n<p><img src=\"http://google.com\" alt=\"alt text\" title=\"alt text\" /></p>\n<p>A list</p>\n<ul><li>apples</li>\n<li>oranges</li>\n<li>eggs</li>\n</ul>\n<p>A numbered list</p>\n<ol><li>a</li>\n<li>b</li>\n<li>c</li>\n</ol>\n<p>A little quote</p>\n<blockquote>It is now time for all good men to come to the aid of their country. </blockquote><p>A final paragraph.</p>\n", output.c_str());
		}
	};
}