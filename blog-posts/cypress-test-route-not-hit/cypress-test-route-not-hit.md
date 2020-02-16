---
published: false
title: "[Cypress] Testing that a route is not hit"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts//cypress-test-route-not-hit/assets/header.png"
description: "How would you test that something does not happen? You may know how to do this but I show you a way you might not know."
tags: cypress, testing, spy, javascript
series: Cypress tricks
canonical_url:
---

This is a little series about tricks that I use in my daily work.

# Problem

Let's say you have a page and you want to test if a click onto a button triggers a `XHR` request. This is easy. Just use:

```js
cy.server()
cy.route("GET", /.*search.php.*/).as("alias")
cy.get("button").click().wait("@alias");
```

But imagine you want to check that the buton click does **NOT** trigger a request? How would you do this?

# Solution

You might come up with this idea:


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
