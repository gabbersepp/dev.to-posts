module.exports = {
  reporters: [
      "default",
      "jest-junit",
      "jest-html-reporter",
      "relative/path/to/jest-report-wrapper.js"
  ],
  testMatch: ["<rootDir>/specs/*.js"]
};
