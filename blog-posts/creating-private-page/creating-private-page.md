---
published: false
title: "Creating a personal page"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/intellisense-for-cypress-fixture-files/assets/your-asset.png"
description: "TODO: fill"
tags: cypress, javascript, vscode, productivity
series: creating_private_page
canonical_url:
---
wichtig: front_matter bearbeiten

Long time ago I had a personal page that provides tutorials about many programming stuff. E.g. I showed how an oprating system can be built using Assembler. Or how the Win32 API can be used with ASM. Also some Delphi stuff was there and so on.

My page was very popular within a specific target group. I had good SEO ranks and all that stuff. But at some point in time I lost interest in writing tutorials. And also I had no time left because other (non programming related) projects became more important. 

Now in 2020 things have changed. I finished university in 2016, made a huge progress at work the last year. I started contributing to open source projects and built a few projects on github. Drawing is my new hobby since 2019 and it seems that this will be something I will do for years. I started using Twitter and Instagram more professional to promote my drawings and coding stuff and finally at the end of 2019 I came across dev.to and started to write tutorials again. I think I am now at a point where it makes sense to combine all those activities into one central website so I can provide it to those who are interested in (and of course, maybe exmployers are).

# What my personal page can and what it cannot do
Yes I am one of those poor people who must handle GDPR because I am from Germany. Also we have very strict laws. That are the main reasons for the following points:
+ **No comments**


    A comment functionality would involve a kind of an login. This would involve cookies and all that stuff and thus would force me to be GDPR compliant. To be honest, I can do so much more in the time that would be needed to fully understand the GDPR. And yes, there are many different intepretations and opinions. I don't think that a person like me and you can implement the GDPR 100% correct without asking a lawyer.
+ **But I want comments!**

    Of course people should be able to comment. A tutorial without interaction is senseless. So I will publish my blog posts regarding programming stuff here at dev.to and then link them at my website. No cookies, no accounts but fully functional!
+ **No advertisement**

    In former times I experimented with different types of ads. But in the end this does not generate that much money. Also I would have to register a small business at my local givernment which would make my income tax return much more complicate. So I ended up with no ads at all.
+ **Publish my drawings I post on Twitter**

    Today I have to publish my drawings at two  social media sites: Twitter and Instagram. The latter does already push the post to Facebook. So I have the posts already published anywhere.  Why should I publish them manual to a third consumer? That does not make sense. So I want to fetch them from twitter and publish them.
+ **Easy to create and modify**

    I like markdown syntax. It has something minimalist. Thus that is how I want to write my page.
+ **Git**

    Everything should be open source on github so I can use one of those free CI server and hosting services. Seems like a Win-Win situation. I can get hosting and deployment for free and people can correct my articles, and the community can use my page as a knowledge base.
+ **Project showcase**

    I have a few projects on github.com. Not very spectacular but at least they are my projects and I want them to be shared among the rest of the world. But of course I do not want to maintain a project's description within my personal page.
 
# What you can expect from this blog post series

I want to show you how I create my page. This means, I show you:
+ **how to manage dev.to blog posts using git**

    well one is easy. I followed the guide here:
    {% post maxime1992/manage-your-dev-to-blog-posts-from-a-git-repo-and-use-continuous-deployment-to-auto-publish-update-them-143j %}

+ **fetch tweets and publish them**

    This aims to my drawings. I want to avoid duplicate effort after finishing my work. But it is not restricted to my drawings. It should be generic so I easily can add new topics I want to aggregate on my page.

+ **present specific projects from github**

    My first thought about this topic could be the best maybe. I will add a special markdown file in each project I want to display at my page. If it works, I can show you how this is done.



# Additional Resources
If you are interested in my old Assembler work you can access everything on github.com. A few month ago I have opensourced all. But be aware: I was young and no professional coder and many stuff is commented in german.

{% github gabbersepp/assembler %}


----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 