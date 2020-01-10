module.exports = {
  reporters: [
      "default",
      "jest-junit",
      ["jest-html-reporter", {
        "pageTitle": "Jest Test Report"
        }]
  ],
  testMatch: ["<rootDir>/spec.js"]
};
