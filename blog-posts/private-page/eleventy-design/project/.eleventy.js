module.exports = function(eleventyConfig) {
  eleventyConfig.addCollection("many_data_entries", () => [1,2,3,4,5,6]);
}