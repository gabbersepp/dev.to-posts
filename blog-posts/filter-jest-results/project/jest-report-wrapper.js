class ReporterWrapper {
  constructor(globalConfig, options) {
    this._globalConfig = globalConfig;
    this._options = options;
    this.underlyingReporters = [];

    this.underlyingReporters = options.map(r => {
      const resolved = require(r.underlying);
      return new resolved(globalConfig, r.underlyingOptions);
    })
  }

  onRunStart(runResults, runConfig) {
    this.underlyingReporters.forEach(r => {
      if (r.onRunStart) {
        r.onRunStart(runResults, runConfig)
      }
    })
  }

  onTestResult(testRunConfig, testResults, runResults) {
    const removed = this.processSpecFile(testResults);
    testResults.numPassingTests -= removed;
    runResults.numPassedTests -= removed;
    runResults.numTotalTests -= removed;

    this.underlyingReporters.forEach(r => {
      if (r.onTestResult) {
        r.onTestResult(testRunConfig, testResults, runResults)
      }
    })
  }

  onRunComplete(test, runResults) {
    for (let i = 0; i < runResults.testResults.length; i++) {
      let tr = runResults.testResults[i];
      if (tr.testResults.length == 0) {
        runResults.testResults.splice(i, 1);
        i--;
        runResults.numPassedTestSuites -= 1;
        runResults.numTotalTestSuites -= 1;
      }
    }

    this.underlyingReporters.forEach(r => {
      if (r.onRunComplete) {
        r.onRunComplete(test, runResults)
      }
    })
  }

  processSpecFile(specFile) {
    let removed = 0;
    for (let testIndex = 0; testIndex < specFile.testResults.length; testIndex++) {
      const test = specFile.testResults[testIndex];
      if (test.ancestorTitles.indexOf("Skipped") > -1 || test.title === "Skipped") {
        // test was skipped thus remove it
        specFile.testResults.splice(testIndex, 1);
        removed++;
        // adjust indexCount because 'specFile.testResults.length' has been updated due to the 'splice'
        testIndex--;
      }
    }

    return removed;
  }
}

module.exports = ReporterWrapper;
