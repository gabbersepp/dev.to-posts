---
published: false
title: "[dotnet core] Getting request body of HttpClient requests with dotnet-trace"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/intellisense-for-cypress-fixture-files/assets/your-asset.png"
description: "Introducing a VSCode extension that provides intellisense for cypress fixture file paths"
tags: cypress, javascript, vscode, productivity
series:
canonical_url:
---

# Our problem
We still use the deprecated `NEST` library for interacting wiht elasticsearch in our `C#` code. A few days ago I was required to take a look at the `Put Mapping` request. I do not know if I can enable logging somewhere to get the full request in readable format. If you are as lazy as I am and want to avoid searching the whole internet to find a piece of code which enables request logging in `NEST`, you have two options:
- Wireshark (and similar tools)
- dotnet-trace

Wireshark is great, no doubt. But it also has it's limitations I think. One limitation is traffic went through SSL encryption. I know that we can instruct Wireshark to decrypt the traffic but it requires some steps.
dotnet-trace on the other side has a big advantage. It collects the traffic when it leaves the user application. We are on a much higher network layer which means the data packages have not seen any SSL stuff yet.

Use this command, to start a trace:
`dotnet-trace -p <pid> --providers ....`

Then to the action of interest

You can then open the tracefile in PerfView

One disadvantage of PerfView is that it does not show the full buffer. I do not know if 

#Introduction

Do you had a situation in .net core where you wanted to see the request which is sent by a third party library? 

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
