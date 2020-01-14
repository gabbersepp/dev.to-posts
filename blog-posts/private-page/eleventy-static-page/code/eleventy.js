const fs = require("fs");

module.exports = function(eleventyConfig) {
    const tweetsStr = fs.readFileSync("../preprocessing/twitter/tweets.json").toString();
    const tweets = JSON.parse(tweetsStr);
    console.log(tweetsStr)
    eleventyConfig.addCollection("tweets", () => tweets);
}