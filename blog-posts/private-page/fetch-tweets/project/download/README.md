# Usage

+ define the necessary environment vars (see `twitter.js` for more information)
+ npm install
+ npm run eleventy
+ open http://localhost:8080/

The structure in this example is a bit different:
+ tweets are loaded before the .eleventy.js config is loaded
+ eleventy only reads the tweets from the JSON file

This is necessary to avoid heavy load during eleventy initialization. 