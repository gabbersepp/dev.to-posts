---
published: true
title: "Prevent routing if unsaved changes exist [VueJS]"
description: "Apply a very small hack to prevent the user from accidentally switching to another page without saving changes [VueJS]"
tags: vue, javascript, webdev, beginners
series:
canonical_url:
---

Let's say you have a input field and routes configured:

![](./assets/form.jpg)

How can you avoid that the user navigates to a route if the inputfield contains unsaved text? I made a few tries to find a solution for that but did not found anything. So here is what I have done to solve this problem.

Find the full runnable example here: [Click me!](https://github.com/gabbersepp/dev.to-posts/tree/master/blog-posts/vuejs-avoid-routes/code/example)

# Solution
First, define a new global function:

```ts
let isRouteChangeBlocked: boolean = false;

export function blockRouteChange(set?: boolean): boolean {
    if (arguments.length == 1) {
        isRouteChangeBlocked = !!set;
        return isRouteChangeBlocked;
    }

    return isRouteChangeBlocked;
}

```

Then use it to set the flag after the user make some input:

```ts
  @Watch("input")
  private inputChange(to: string) {
    if (to && to.length > 0) {
      blockRouteChange(true);
    } else {
      blockRouteChange(false);
    }
  }
```

After a successful saving, reset the flag:

```ts
  private save() {
    blockRouteChange(false);
    alert("saved");
  }
```

Now replace the router's `push` function:

```ts
const originalPush = VueRouter.prototype.push;
VueRouter.prototype.push = function(location: RawLocation) {
    if (blockRouteChange()) {
        if (confirm("There are unsaved changes, do you want to continue?")) {
            blockRouteChange(false);
            return originalPush.call(this, location) as any;
        }
        return;
    }
    return originalPush.call(this, location) as any;
};
```

# Result
Here is what I got:

![](./assets/vuejs-route1.gif)

# Conclusion
This workaround works really well but I'm wondering whether there isn't a better possibility? Do you know how this can be achieved using a native solution? Maybe I just was not clever enough to type in the correct search terms into google :sweat_smile: Just let me know.

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
