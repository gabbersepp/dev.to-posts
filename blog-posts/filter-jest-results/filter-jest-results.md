---
published: false
title: "Filter Jest test results based on test result itself using a wrapper"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/intellisense-for-cypress-fixture-files/assets/your-asset.png"
description: "If you ever needed to filter Jest test results based on something that is within the result itself, then this article might helü you. Also this article gives a little insight on how to write a Jest reporter."
tags: jest, testing, javascript, tutorial
series:
canonical_url:
---

Today we stumbled over a problem in our API test infrastructure that induced some brainwork to fix it. In this article I show you how you can build your own Jest reporter and also how you can wrap existing reporters to filter out specific test results.

# Our use case
The usecase is a bit complicated. In the end we needed to remove test results if test are skipped.

If we have this `javascript` code:
```js
// code/jest.spec.js
```

I want to get this test result:


# Additional Resources
+ [A very good explanation of the `Jest` reporter interface](https://medium.com/@colinwren/writing-a-jest-test-reporter-cb7c123ec211)
+ [Empty `Jest` reporter template](https://github.com/colinfwren/jest-reporter-debug/blob/master/src/index.js)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
