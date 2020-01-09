---
published: false
title: "Filter Jest test results based on test result itself using a wrapper"
cover_image: "https://raw.githubusercontent.com/gabbersepp/dev.to-posts/master/blog-posts/filter-jest-results/assets/header.png"
description: "If you ever needed to filter Jest test results based on something that is within the result itself, then this article might helü you. Also this article gives a little insight on how to write a Jest reporter."
tags: jest, testing, javascript, tutorial
series:
canonical_url:
---

Today we stumbled over a problem in our API test infrastructure that induced some brainwork to fix it. In this article I show you how you can build your own Jest reporter and also how you can wrap existing reporters to filter out specific test results.

# Our use case
We skip tests conditional under specific circumstances. To keep our reports clean we want to filter out skipped tests. In our case a skipped test contains only the term **Skipped** in it's name. So we want to omit all tests that are named **Skipped**.

**This is our spec file:**

wichtgi: nur grüne fälle nutzen, weil wir später im code auch nur die success subtrahieren

```js
// code/jest.spec.js

describe("suite", () => {
  it("this test should appear in the html and junit report", () => {
    expect(true).toBe(true)
  })

  it("Skipped", () => {
    expect(false).toBe(true)
  })
})

```

**And the `jest` config:**

```js
// code/jest.config-before.js

module.exports = {
  reporters: [
      "default",
      "jest-junit",
      ["jest-html-reporter", {
        "pageTitle": "Jest Test Report"
        }]
  ],
  testMatch: ["<rootDir>/specs/*.js"]
};

```

Which results in following results:

**HTML**:

![HTML report before filtering](assets/html-report-before.PNG)

**JUNIT**:

```xml
<!-- code/junit-before.xml -->

<?xml version="1.0" encoding="UTF-8"?>
<testsuites name="jest tests" tests="2" failures="1" time="2.823">
  <testsuite name="suite" errors="0" failures="1" skipped="0" timestamp="2020-01-07T20:10:48" time="1.584" tests="2">
    <testcase classname="suite this test should appear in the html and junit report" name="suite this test should appear in the html and junit report" time="0.002">
    </testcase>
    <testcase classname="suite this test should not appear in the html and junit report" name="suite this test should not appear in the html and junit report" time="0.003">
      <failure>Error: expect(received).toBe(expected) // Object.is equality
</failure>
    </testcase>
  </testsuite>
</testsuites>
```

Obviously we want this output:

TODO: add output

# The idea
I read through the "Jest" documentation, but found nothing to do with any filter logic. We also don't want to change existing reporters. So the only solution is to pack an existing reporter into an own reporter that does not pass on all test results. Also some test measurements must be adjusted, like the amount of total tests and so on.

# Writing an Jest reporter
So our first goal is to write an own jest reporter. It's basic structure is very simple as you can see:

```js
// code/jest-reporter-empty.js

class ReporterWrapper {
  constructor(globalConfig, options) {
  }

  onRunStart(runResults, runConfig) {
  }

  onTestResult(testRunConfig, testResults, runResults) {
  }

  onRunComplete(test, runResults) {
  }
}

module.exports = ReporterWrapper;

```

Writing a reporter is simple. You just do anything you want within those methods, save the file anywhere and then reference your reporter in the `jest.config.js` among the other reporters:

```js
// code/jest.config-own-reporter.js

module.exports = {
  reporters: [
      "default",
      "jest-junit",
      "jest-html-reporter",
      "relative/path/to/jest-report-wrapper.js"
  ],
  testMatch: ["<rootDir>/specs/*.js"]
};

```

:exclamation: **Attention**:
`Jest` passes the same instance of test results through all reporters. And it seems that the list of reporters is worked off beginning with the first. So **changes** made to the results in the first reporter are **visible in the subsequent reporters**, too.

# The structure of the test results
> **_NOTE:_** 
I don't want to go to deep into the meaning of the arguments because there are many good resources in the web. Please see the [Additional Resources](#additional-resources) section for further information.

`testResults` contains the result of the last executed spec file. `runResults` contains all results from within one execution. The structure (only the relevant parts) of the `testResults` looks like this:

<!-- embedme code/testResults.json -->
```json
{
  "numPassingTests": 5,
  "testFilePath": "path/to/spec.js",
  "testResults": [
    {
      "ancestorTitles": [
        "SelectField Component"
      ],
      "title": "renders text input correctly"
    }
  ]
}

```
`ancestorTitles` contains a list of parent names. They come from `descibe()` and nested `describe()`. 

And the `runResults`'s structure looks like this:

<!-- embedme code/runResults.json -->
```json
{
  "numPassedTestSuites": 15,
  "numTotalTestSuites": 15,
  "numPassedTests": 46,
  "numTotalTests": 46,
  "testResults": [
    "here are testResults"
  ]
}

```

# Cleaning the test results
The only thing we have to do is

# Additional Resources <a name="additional-resources">
+ [A very good explanation of the `Jest` reporter interface](https://medium.com/@colinwren/writing-a-jest-test-reporter-cb7c123ec211)
+ [Empty `Jest` reporter template](https://github.com/colinfwren/jest-reporter-debug/blob/master/src/index.js)

----

# Found a typo?
As I am not a native English speaker, it is very likely that you will find an error. In this case, feel free to create a pull request here: https://github.com/gabbersepp/dev.to-posts . Also please open a PR for all other kind of errors.

Do not worry about merge conflicts. I will resolve them on my own. 
