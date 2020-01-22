const request = require("request");
const fs = require("fs");

function download(twitterUrl, id) {
    const ext = twitterUrl.split("\.").reverse()[0];
   
    return new Promise(resolve => {
        const path = `/data/${id}.${ext}`;
        const stream = fs.createWriteStream(`.${path}`);
        request(twitterUrl).pipe(stream);
        stream.on("finish", () => resolve(path));
    });
}

module.exports = download;