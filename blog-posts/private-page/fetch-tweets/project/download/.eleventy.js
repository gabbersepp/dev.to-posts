const fs = require("fs");

module.exports = function(eleventyConfig) {
  eleventyConfig.addPassthroughCopy("./data");
  const tweets = JSON.parse(fs.readFileSync('./data/tweets.json').toString())
  eleventyConfig.addCollection("tweets", () => tweets);
}