KotoParser
======================

KotoParser is a C++ kotodown(a variation of markdown) parser which is completely rewritten from [MarkdownDeep](https://github.com/toptensoftware/markdowndeep)'s code.

KotoParser aims at these objectives:

* **Fast**. KotoParser must be fast in order to serve as a part of an software.
* **Accurate**. KotoParser should not allow wrong results.

Usage
---------------------

```cpp
#include <iostream>
#include "kotoparser.h"

using namespace std;

int main()
{
    cout << kotoparser::transform(L"Hello, **markdown**");
    // output: <p>Hello, <b>markdown</b></p>
    return 0;
}
```

Currently supported grammar
----------------------

### Block grammar

#### Heading

```
# Hi, this is a heading!
```

#### Blockquote

```
> Hi, this is a blockquote!
```

#### Code Block

    ```
    Hi, this is a code block!
    ```

#### Definition Lists (MediaWiki-style)

```
; Term : Definition
; Term 2
: Definition 2
```

#### Unordered & Ordered Lists

```
1. This is an ordered list.
2. Second item
* This is an unordered list.
* Second item
```

#### Paragraph

```
This is the first paragraph.

This is the second paragraph.
```

### Inline grammar

#### Emphasize

```
Hello, **markdown**!
Hello, *world*!
```

#### Inline Code

```
`code`
```

#### Inline HTML

```
<span>Hello</span>
```

#### Link

```
[Hello](Hello)
[[Hello]]
```

#### Image

```
![Image Alternative Text](Image Link)
```