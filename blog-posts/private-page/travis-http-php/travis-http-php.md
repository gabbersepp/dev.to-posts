---
published: true
title: "Publishing my blog using HTTP upload in PHP"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/private-page/travis-http-php/assets/header.jpg"
description: "After a hard struggle with travis and my FTP server, I decided to use a HTTP upload"
tags: php, javascript, website, deployment
series: creating_private_page
canonical_url:
---

In the last article I wrote about how to publish a website with `travis` and FTP. First everything seemed fine but the nightly build suddenly failed. It took long time until I realized that this was not because of my code or my `ftp server` but because of how travis has setup it's network layers. Read on here if you are interested: https://blog.travis-ci.com/2018-07-23-the-tale-of-ftp-at-travis-ci

But the fight is not lost! My webspace paket includes a PHP instance and thus I am able to write a small HTTP upload tool. A bit oversized I think but it enables me to continue using my webspace bundle.

# The PHP fileupload
Shame on me, it's been a long time since I programmed PHP. So I guess the following code is written very quick and dirty.

First I need a method for reading the `HTTP` Header to check a secret that I send along with the request.

```php
// code/upload.php#L3-L19

function getRequestHeaders() {
    $headers = array();
    foreach($_SERVER as $key => $value) {
        if (substr($key, 0, 5) <> 'HTTP_') {
            continue;
        }
        $header = str_replace(' ', '-', ucwords(str_replace('_', ' ', strtolower(substr($key, 5)))));
        $headers[$header] = $value;
    }
    return $headers;
}

$headers = getRequestHeaders();

if ($headers['Secret'] !== "<your secret>") {
	die("wrong secret");
}
```

The file can be accessed with `$_FILES`. To store the image somewhere, use `move_uploaded_file`.

```php
// code/upload.php#L21-L22

move_uploaded_file($_FILES['zip-file']['tmp_name'], './'.$_FILES['zip-file']['name']);

```

It is very basic but should be enough to accept files from anywhere. To speed up the upload process I moved the whole `/dist`directory into a ZIP archive. So I need to unzip it with PHP:

```php
// code/upload.php#L23-L30

$zip = new ZipArchive;
if ($zip->open('test.zip') === TRUE) {
    $zip->extractTo('./');
    $zip->close();
    echo 'ok';
} else {
    echo 'error duringunzip';
}
```

# Zip & send the files with NodeJS
For zipping the files I use [archiver](https://www.npmjs.com/package/archiver) and for making the upload request [request](https://www.npmjs.com/package/request).

`archiver` is very straightforward and only needs a few lines of code:

```js
// code/zip.js

var fs = require('fs');
var archiver = require('archiver');

var fileName =   'test.zip'
var fileOutput = fs.createWriteStream(fileName);
const archive = archiver('zip');

fileOutput.on('close', function () {
    console.log(archive.pointer() + ' total bytes');
    console.log('archiver has been finalized and the output file descriptor has closed.');
});

archive.pipe(fileOutput);
archive.directory('dist/', false);
archive.on('error', function(err){
    throw err;
});
archive.finalize();
```

Sending the file is also very simple and done quickly:

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

# Summary
I replaced the FTP deployment with a HTTP upload endpoint. The `/dist` directory is zipped and unzipped with `php`. This was required because FTP upload does not work with travis very well. 


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 