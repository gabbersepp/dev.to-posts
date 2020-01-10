---
published: false
title: "Use eleventy to create my static page"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/eleventy-static-page/assets/header.png"
description: "Write your website with markdown + liquid and publish it as static page. This article shows how I create my page with eleventy"
tags: tutorial, eleventy, html, website
series: creating_private_page
canonical_url:
---

>**Example files:** You'll find the example code [in the blog post's project directory](./project/README.md)

This is the second part of this series. In the first I talked about my motivations to revive my old website [biehler-josef.de](https://biehler-josef.de). Now we need to build the first milestone and this is definitely the HTML that everyone can view in the browser. 
I thought a lot about how to do this. For those that just began to code the last one to ten years, I can tell you that today you have the best time ever. Back then there were only a few tutorials, nearly nothing complex with **javascript**, **PHP** was very popular and so were PHP based CMS systems.
Nowadays this has changed completely. You have plenty of possibilities to build a website. You can choose whatever JS framework you want. You can host your page, whether you use NodeJS or C# or ... , with very low cost or even for free if you publish your project der an open source license.

# Choosing the right thing
Among all those possibilities it is very hard to decide how I should create my website. I already have [one](https://jodekadev.de) that is written with React but completely manually. This is sufficient because that content does not change very often. But I plan to updated the content of the new page every day or at least once per week. 
By chance I discovered [11ty](https://11ty.dev) a few days ago. It is a static site builder that works with zero config. I hate tools I have to configure very long. In the best case I install it and it works. And yes, **11ty** fulfills exactly my expectation!

This are the steps to use `eleventy`:
+ install it: `npm install -D @11ty/eleventy`
+ create a npm script: 
```
"scripts": {
    "11ty": "eleventy"
  }
```
+ create a `.md` file
+ call `npm run 11ty`

And after that you will find a directory named `_site` that contains the **HTML** code.

> **_NOTE:_** You also can start a watch task to enable `11ty` to automatically observe your `.md` files. See the sample project files for this blog post.

# Display tweets in the page
Let's say we have a file `tweets.json` that contains a list of tweets and we want to display them. How can we do this? **Eleventy** sends the markdown files through the [liquid template engine](https://shopify.github.io/liquid/) so we can use everything we can do in liquid. That means we have control structures and iteration structures as well.

So something like this will do the trick:

```
{% for tweet in tweets %}
  id: {{ tweet.id }}
{% endfor %}
```

# Publish a new dynamic collection to liquid
In order to access `tweets` we must read the file and tell someone that we have a new list of objects.

The reading part is easy:
```js
    const tweetsStr = fs.readFileSync("../preprocessing/twitter/tweets.json").toString();
    const tweets = JSON.parse(tweetsStr);
```

Eleventy offers a special method that allows you to add new collections. To use this you must create a file named `.eleventy.js` in the project root (it is a config file) that exports a function whose only parameter is a `eleventyConfig` object.
On that object you can call `addCollection('tweets', tweets)`. Now you can access it:

```
{% for t in collections.tweets %}
    {{ t.id }}
{% endfor %}
```

Note the **collections** keyword here which is necessary.

Full code of `.eleventy.js`:

```js
// code/eleventy.js

const fs = require("fs");

module.exports = function(eleventyConfig) {
    const tweetsStr = fs.readFileSync("../preprocessing/twitter/tweets.json").toString();
    const tweets = JSON.parse(tweetsStr);
    console.log(tweetsStr)
    eleventyConfig.addCollection("tweets", () => tweets);
}
```

# Examplecode
You can checkout the my blog post repo and test the code used in this post. Check the [README.md](./project/README.md) of the project.

# Summary
You have learned how easy `Eleventy` can be setup and how you can utilize Liquid to display a dynamic list. 

# What's next
Now as we have HTML code, I have to publish it somewhere. In the next article I'll show you how everything can be deployed to a webspace using `travis` and a FTP server. Hopefully this works, otherwise I must choose another strategy :sweat_smile:

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
