---
published: false
title: "Intellisense for Cypress Fixture Files"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/intellisense-for-cypress-fixture-files/assets/your-asset.png"
description: "Introducing a VSCode extension that provides intellisense for cypress fixture file paths"
tags: cypress, javascript, vscode, productivity
series:
canonical_url:
---

In the last blog post I introduced you into the power of `eleventy`'s pagination feature. I also gave you a very simple navigation. It looks like this:

TODO: add picture

But wouldn't it be nice if it looks like this:

TODO: add picture

The advantages are obvious:
+ you easily can jump to the first and last page
+ you see how many pages exist
+ you must not display the whole navigation bar with all pages
+ you have the page befopre the current and the page after the current in the navigation bar which looks a bit better than without it

As in the most cases, I use a `nunjucks` template for this example. But you can apply this approach in other engines, too.

# Pagination's properties

When you declare a pagination in teh frontmatter, `eleventy` makes a variable named `pagination` available in your template. Amongst other things you have access to those properties:

use pagination.hrefs instead of pagination.pages
```json
{
  "items": "the sliced window containing only the subset of the whole data set",
  "href": {
    "previous": "url of the previous page",
    "next": "url of the next page"
  },
  "pages": "",
  "hrefs": "array with all URL's of this pagination"
}
```

When iterating over a set of data, you probably do this by:

```
  {% for item in items %}
```

Within the loop scope you have access to a variable named `loop` with following properties:

```json
{
  "index": "current 1-based index",
  "index0": "current 0-based index"
}
```


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
