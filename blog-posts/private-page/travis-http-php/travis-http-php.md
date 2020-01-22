---
published: false
title: "Publishing my blog using HTTP upload in PHP"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/private-page/travis-http-php/assets/header.png"
description: "After a hard struggle with travis and my FTP server, I decided to use a HTTP upload"
tags: php, javascript, website, deployment
series: creating_private_page
canonical_url:
---

In the last article I wrote about how to publish a website with `travis` and FTP. First everything seems fine but the nightly build suddenly failed. It took long time until I realized that this was not because of my code or my `ftp server` but because of how travis has setup it's network layers. Read on here if you are interested: https://blog.travis-ci.com/2018-07-23-the-tale-of-ftp-at-travis-ci

But the fight is not lost! My webspace paket includes a PHP instance and thus I am able to write a small HTTP upload tool. A bit oversized I think but it enables me to continue using my webspace bundle.

# The PHP fileupload
Shame on me, it's been a long time since I programmed PHP. So I guess the following code is very quick and dirty.

```php
// code/upload.php
```

It is very basic but should be enough to accept files from anywhere. To speed up the upload process I moved the whole `/dist`directory into a ZIP archive. So I need to unzip it with PHP:

```php
// code/unzip.php
```

# Zip & send the files with NodeJS
I was a bit lazy and took the first search result on google to get a runnable code for uploading a file.

```js
// code/send.js

const request = require("request");
const fs = require("fs");
const path = require("path");
var options = {
    url: 'https://biehler-josef.de/upload.php',
    headers: {
        secret: process.env.JB_UPLOAD_SECRET
    }
}
var r = request.post(options, function optionalCallback (err, httpResponse, body) {
  console.log('Server responded with:', body, err);
})
var form = r.form()
form.append('zip-file', fs.createReadStream(path.join(__dirname, "..", 'test.zip')))
```


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 