const request = require("request");
const fs = require("fs");
const path = require("path");
var options = {
    url: 'https://biehler-josef.de/upload.php',
    headers: {
        secret: process.env.JB_UPLOAD_SECRET
    }
}
var r = request.post(options, function optionalCallback (err, httpResponse, body) {
  console.log('Server responded with:', body, err);
})
var form = r.form()
form.append('zip-file', fs.createReadStream(path.join(__dirname, "..", 'test.zip')))