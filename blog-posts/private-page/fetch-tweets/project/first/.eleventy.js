const getTweets = require("./twitter");

module.exports = function(eleventyConfig) {
  eleventyConfig.addCollection("tweets", () => getTweets());
}