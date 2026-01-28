---
published: false
title: "Cause of a missing column in the include section of an index"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/reports-from-working-life/missing-include-in-index/assets/your-asset.png"
description: "One missing include column can can ruin your evening."
tags: sql, performance, index
series:
canonical_url:
---

# The Problem

Last week I stumbled across a common situation. After we deployed an adaption of a feature, several customers complained about bad performance of the application.

# The analysis

The very first action you can do in such a situation is to open the profiler, start it and hope you will see some queries that take very long though they should not. In my case I saw such queries.

The next step is now to get the execution plan for those queries. This was the result:

--imager--

We see a index scan. This may not



# Important

I never start optimizations of queries by looking at the execution plan. My first step is always to look at the code, understand the code and see if it can be written better to get a better query. But in this case I knew that we had adjusted code that may affect existing indices.

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
