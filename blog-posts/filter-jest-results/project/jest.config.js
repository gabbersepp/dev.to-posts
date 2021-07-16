module.exports = {
  reporters: [
    "default",
    ["./jest-report-wrapper.js",
      [
        { underlying: "jest-junit" },
        {
          underlying: "jest-html-reporter",
          underlyingOptions: {
            "pageTitle": "Jest Test Report"
          }
        }
      ]
    ]
  ],
  testMatch: ["<rootDir>/spec.js"]
};
