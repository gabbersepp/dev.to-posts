
const Handlebars = require("./../../project/node_modules/handlebars");
const fs = require("fs");

const output = require(`./mocha-output.json`);

const template = fs.readFileSync("./mocha-html-report-template.html", "utf8");
const htmlOutput = Handlebars.compile(template)(output);

fs.writeFileSync("./cypress-result.html", htmlOutput);
