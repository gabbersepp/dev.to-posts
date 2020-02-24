
module.exports = function(eleventyConfig) {
  eleventyConfig.addCollection("manyData", async () =>
    new Promise(resolve => {
      const results = [];
      for(var i = 0; i < 100; i++) {
        results.push({
          id: i,
          text: `item ${i}`
        });
      }
      resolve(results);
    })
  );

  return {
    dir: {
      input: "./views"
    }
  }
}
