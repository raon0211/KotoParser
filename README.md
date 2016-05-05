KotoParser
======================

KotoParser is a C++ markdown parser which is completely rewritten from [MarkdownDeep](https://github.com/toptensoftware/markdowndeep)'s code. It got many ideas and algorithms from it.

KotoParser aims at these objectives:

* **Fast**. KotoParser must be fast in order to serve as a part of an software.
* **Accurate**. KotoParser should support all markdown features, and should not allow wrong results.

Usage
---------------------

```cpp
#include <iostream>
#include "kotoparser.h"

using namespace std;

int main(void)
{
    cout << kotoparser::transform(L"Hello, **markdown**");
    // output: <p>Hello, <b>markdown</b></p>
    return 0;
}
```