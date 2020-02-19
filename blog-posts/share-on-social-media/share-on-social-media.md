---
published: false
title: "Intellisense for Cypress Fixture Files"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/intellisense-for-cypress-fixture-files/assets/your-asset.png"
description: "Introducing a VSCode extension that provides intellisense for cypress fixture file paths"
tags: cypress, javascript, vscode, productivity
series:
canonical_url:
---

In my spare time I'm drawing cartoons. After finishing one, of course I want to publish it on several social media platforms:
+ Facebook
+ Instagram
+ Twitter

On every platform I have a personal profile where I publish everything and a profile for cartoons related to developer topics [KackDev](https://kack.dev). 

My workflow currently is:
+ Publish the post at Instagram @KackDev
+ Publish the same at twitter @kackDev
+ Repost the twitter post @JosefBiehler
+ Repost the Insta in my story at @JosefBiehler
+ Publish the post at Instagram @JosefBiehler

Wow! Many steps that has to be done manually before I can annoy everyone with my bad drawings! 

I was asking myself if this can't be automated. Can it? Maybe!

# IFTTT
[**I**f **t**his **t**han **t**hat](https://ifttt.com) is a service where you can define workflows based upon start actions. E.g. "if I receive an email from xxx then send one to yyyy" and stuff like that.

It also provides Twitter, Instagram and Facebook integration. And it is free! Whoa!

**Accounts**

`IFTTT` allows only one Twitter/Instagram/Facebook connection per IFTTT account. As we want to control two social media profiles per platform, we need two `IFTTT` acccounts. In the remaining blog post I use the term `private IFTTT account` for that one that is connected to my private social media profiles. Accordingly I call my account that is linked to my `KackDev`social media profiles `KackDev IFTTT account`.

# Publishing at Instagram @KackDev
I use Planoly for this task. But it is not required. Planoly allows me in the free tier to maintain one Instagram account and create many hashtag sets. The later is the reason why I fall back on a third party tool. I do not want to search the hashtags all the time I make a post.

# Publishing the same at Twitter @KackDev
Now I need a Insta<->Twitter integration in my KackDev IFTTT account. Luckily someone has published such an applet at `ifttt`.

## Small IFTTT excurse
If you are new to IFTTT, your start screen will look like this:

![Startscreen](./assets/ifttt-1.jpg)

Klick onto the magnifying glass at the top and type in "twitter". You will see many results.

![Results](./assets/ifttt-2.jpg)

We are interested in the second applet "Tweet your Instargams as native photos on Twitter". Click onto it. Press the big "Connect" Toggle. Now `ifttt` will redirect you to instagram which will ask you to authorize `ifttt`. In my case I link  my KackDev Instagram account with my KackDev IFTTT account. Now You will be redirected to IFTTT which then redirects you to Twitter. Here I link my KackDev Twitter account.

If you see following screen, everything works as expected:

![finished](./assets/ifttt-3.jpg)

## Finishing the integration
Well, nothing more has to be done. I have activated the applet, linked both social media accounts to my KackDev IFTTT account.

## Testing the integration
I upload any picture to my KackDev account

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 