---
published: false
title: "The impact of branch predictions in modern CPU architectures"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/impact-of-branch-predictions/assets/header.jpg"
description: "Have you already heard of `branch predictions`? They may have a high impact on algorithms working on big datasets."
tags: internals, performance, cpu, dotnet
series:
canonical_url:
---

# What are branch predictions?
Let's say you are a taxi driver and you have to drive every day the same person to the same place. Every time the passenger asks you to turn left on a very big crossing. After a few days, you know that automatically. This will save you time because you can very early change the lane on the road. But what if the passenger suddenly asks you to turn right this time? Then you must "revert" your decision and change the lane again.
This is how branch predictions work. if the CPU detects a certain pattern within your `if` clauses, it will load the next commands into the memory. If the `if` evaluates to the expected result, this was a time saver because the expensive operation of loading something into the memory was already done. If the decision was wrong, the CPU had to load other instructions.

# See this in action
Well, nobody of us can have a look into a CPU and monitor when a prediction took place. At least I do not know those techniques. If you do, let me know!

But we can build a setup that should take advantage of branch predictions.

>**Note:** You should compile the following C# code with the `Debug` profile to get exactly what we have written down. The `Release` build will optimize your code and thus have side effects. Read this short introduction if you want to know more about this:
{% link https://dev.to/gabbersepp/differences-between-release-and-debug-build-assembler-code-included-52k6 %}

```cs
// ./code/Program.cs#L8-L69

static void Main(string[] args)
{
    var sw = new Stopwatch();
    sw.Start();
    SameBranch();
    sw.Stop();
    var sameBranchTime = sw.ElapsedMilliseconds;

    sw.Reset();
    sw.Start();
    DifferentBranch();
    sw.Stop();
    var differentBranchTime = sw.ElapsedMilliseconds;

    Console.WriteLine($"different-branches: {differentBranchTime}, same-branch: {sameBranchTime}");
    Console.Read();
}

static int DifferentBranch()
{
    int result = 0;

    for (var i = 0; i < 1000000000; i++)
    {
        if (i > Random(0, 1000000000))
        {
            result = i;
        }
        else
        {
            result = i;
        }
    }

    return result;
}

static int SameBranch()
{
    int result = 0;

    for (var i = 1000000000; i < 2000000000; i++)
    {
        if (i > Random(0, 1000000000))
        {
            result = i;
        }
        else
        {
            result = i;
        }
    }

    return result;
}

static Random random = new Random();

static int Random(int from, int to)
{
    return random.Next(from, to);
}
```

## How I expect `DifferentBranch` to work
`SameBranch()` will generate a random number between zero and 1,000,000,000 and thus the number is always lower than `i`. This should force the CPU to use branch predictions and we should see a better performance then `DifferentBranch()`. There a number between 1,000,000,000 and 2,000,000,000 is chosen. If we assume that `Random.Next()` produces evenly distributed random numbers, we should see the branch to be true for half of the time. This should be enough to disturb the branch prediction and to produce a worse runtime as the other example.

## Results
I get these results for five runs of the text program:

```
different-branches: 43745, same-branch: 39822
different-branches: 45647, same-branch: 41163
different-branches: 43938, same-branch: 39917
different-branches: 44935, same-branch: 41419
different-branches: 43378, same-branch: 39110
```

In average the part with a successful branch prediction is about 10% faster than the other version.

# Conclusions
I don't think that this is something that you have to be worried about in your daily work except you are working on some critical algorithms. But nevertheless this is something that should be in your mind when developing a new algorithm for something.

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 