---
published: false
title: "Low level access to Chrome Debugger Protocol and Cypress"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/cypress-cri/assets/header3.png"
description: "Show Low level access to the chrome remote debugger protocol and an introduction how to the protocol with an abstraction layer with cypress"
tags: cypress, testing, debug, chrome
series:
canonical_url:
---

The Chrome Debugger Interface offers some features that sometimes save your time. This article shows you the low level API as well as links to additional resources. The latter also contain an official cypress example (written by me, so you can ask me questions about it in the comment section, too).

# My real world use case
A few months ago, a friend of mine wanted a digital replacement of the so called *Serviceheft*.
If you fix something on the car, you make a new entry with the current date and all the stuff you fixed. Also you can check off predefined list entries. I am not sure how this is called in English. Let's name it **service history**. I implemented it with [`React` and `Electron`](https://github.com/gabbersepp/service-history). 

> **_NOTE:_** 
Currently I'm refactoring the whole application. So you will find not the best code in this repository :-D

See this example Screen (it is in German right now):
![Service History example](./assets/service-history-example.jpg)

# Usage of Print Media Query
Maybe you noticed the *Print* button at the bottom of the app. It will print the whole page. But as you can imagine, when the entry gets printed, I do not want some elements to appear at the paper. E.g. the images at the bottom. Also I do not want the input elements to show up as such. And the textarea should be resized to show the whole content. 

This is done by using a `print media query` that defines some extra CSS. The end result looks like this:

![Print view](./assets/service-history-print.jpg)

# Cypress tests
As I mentioned, I am refactoring the whole application. Typescript replaces all JS code, I replace all single elements by my own react component library and so on. Of course before doing this, I have written some tests to ensure that everything after the refactoring works exactly like before. Also the print stuff should be tested.

## Print view test
But how can this be accomplished? You can not set media queries with JavaScript and so you can not do this with `Cypress`. 
But if you take a look at your Chrome Dev Console, you maybe know the `Run Command` action:

![commands](./assets/chrome-dev-commands.jpg)

This opens a list of all available commands, including one for emulation the print query:

![print query](./assets/chrome-dev-print.jpg)

The good news is that as far as I know everything you can access from here can also be done programmatically with the `Chrome Remote Debugger Protocol`.
The bad news is that often it is hard to discover how this must be done.

# Chrome Remote Debugger protocol
You communicate over network and calling HTTP endpoints. This can be done with plain JS code but it is recommended to use an abstraction layer so you do not need to worry about the concrete endpoints.

## Simple example with low level access
For the manual test I have used the extension [simple-websocket-client](https://chrome.google.com/webstore/detail/simple-websocket-client/pfdhoblngboilpfeibdedpjgfnlcodoo?hl=de) which provides a lightweight generic websocket client but you can choose the client of your choice.

Follow these steps to navigate to a new URL using the protocol:
+ close all chrome windows (this is necessary otherwise chrome does not start in debug mode)
+ open Chrome through the commandline and specify the debug port: `chrome.exe --remote-debugging-port=9222` (9222 is the default port and can be changed)
    Chrome should be installed here: `C:\Program Files (x86)\Google\Chrome\Application`
+ open a new Chrome tab and navigate to https://google.com
+ open http://localhost:9222/json (can be done in the newly opened window or within another browser or through an ajax call)
    this will list all possible debug targets, including all extensions and your newly created tab **google.com**. In my case this is, what I get:
    ```json
    ...
    {
        "description": "",
        "devtoolsFrontendUrl": "/devtools/inspector.html?ws=localhost:9222/devtools/page/24B0DBC39A658BD7E26B5A4DCB704F88",
        "faviconUrl": "https://www.google.com/favicon.ico",
        "id": "24B0DBC39A658BD7E26B5A4DCB704F88",
        "title": "Google",
        "type": "page",
        "url": "https://www.google.com/",
        "webSocketDebuggerUrl": "ws://localhost:9222/devtools/page/24B0DBC39A658BD7E26B5A4DCB704F88"
    }
    ...
    ```

+ Here you need the websocket URL. You have to connect to this URL in order to control the tab.
    So open the mentioned extension by clicking the small icon: 
    ![WS Client](./assets/ws-client-icon.jpg)
+ Copy & paste the WS URL into the client and click *open*:
    ![WS Client Connect](./assets/ws-client-connect.jpg)
    Now you should be able to send messages to the tab.
+ Paste this JSON into the *Request* input and click *Send*:
    ```json
    {
        "id": 1,
        "method": "Page.navigate",
        "params": {
            "url": "https://example.com"
        }
    }
    ```
+ The tab should have changed the URL to http://example.com

> **_NOTE:_**  *id* specifies an unique id to identify this message. I think it is only used in the response so that the developer can find the corresponding request to the incoming response.

## Using a wrapper around the websocket implementation
Writing down all those WS requests can be a little bit cumbersome. So it is recommended to use an existing wrapper. I use the [chrome-remote-interface](https://github.com/cyrus-and/chrome-remote-interface) package. Please go to it's [README.MD](https://github.com/cyrus-and/chrome-remote-interface/blob/master/README.md) to learn more about it's usage.

# Using it during your tests
There are some limitations when you use the protocol in your tests.
+ **Can be used with chrome only**

    You are not able to use those features when running the tests in electron because it does not fully implement the protocol right now
+ **Headless Chrome can only be used since Cypress 3.8.1**

    Normally when Chrome is used for Cypress, Chrome is started headed. But since [3.8.0](https://docs.cypress.io/guides/references/changelog.html#3-8-0) you can start Chrome headless. A Bugfix released in 3.8.1 is necessary to fix the headless mode for all clients. This means, you can not use it with Cypress before 3.8.0.
+ **Conflict with existing connection**

    If Cypress is started with `--browser chrome` parameter, a debug port is passed automatically. This means you can not specify the port you wish. A little trick is needed to resolve this.
+ **Resetting the chrome state**

    Whenever a spec file contains more than one test, the same browser instance is reused. This means, that you also use the same debugging session. And this introduces problems. Let's say, you activate the `print media query` in the first test which hides an element. Then in the next test, this element is missing, too. This holds true also if you reload the page. To bypass this behavior you need to reset the browser state before or after each test which can lead to further problems.

## Full runnable example
You can copy & paste all the code shown here or just use the [official example](https://github.com/cypress-io/cypress-example-recipes/pull/384) added by me to the official example recipes repo.


# Additional links
- [Simple Websocket Client Extension](https://chrome.google.com/webstore/detail/simple-websocket-client/pfdhoblngboilpfeibdedpjgfnlcodoo?hl=de)
- [chrome-remote-interface](https://github.com/cyrus-and/chrome-remote-interface)
- [Full protocol documentation](https://chromedevtools.github.io/devtools-protocol/)
- [Runnable example (Cypress)](https://github.com/cypress-io/cypress-example-recipes/pull/384)

----

# Found a typo?
As I am not a native english speaker, it is very likly that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 