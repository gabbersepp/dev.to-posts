// Heavily copy-pasted from: https://github.com/mochajs/mocha/blob/master/lib/reporters/json.js

'use strict';

const fs = require("fs");
const mocha = require('./../../project/node_modules/mocha');

exports = module.exports = JSONReporter;

function JSONReporter(runner, options) {
    mocha.reporters.Base.call(this, runner);
    var suites = [];

    runner.on('suite end', function (suite) {
        suites.push(suite);
    });

    runner.once('end', function () {
        var obj = {
            suites: cleanCycles(suites)
        };

        fs.writeFileSync("mocha-output.json", JSON.stringify(cleanCycles(suites[0]), null, 2));
    });
}

function cleanCycles(obj) {
    var cache = [];
    return JSON.parse(
        JSON.stringify(obj, function (key, value) {
            if (typeof value === 'object' && value !== null) {
                if (cache.indexOf(value) !== -1) {
                    // Instead of going in a circle, we'll print [object Object]
                    return '' + value;
                }
                cache.push(value);
            }

            return value;
        })
    );
}