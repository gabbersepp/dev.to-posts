module.exports = {
  reporters: [
      "default",
      ["jest-report-wrapper.js", [
        "jest-junit",
        ["jest-html-reporter", {
          "pageTitle": "Jest Test Report"
          }]
      ]]
  ],
  testMatch: ["<rootDir>/specs/*.js"]
};
