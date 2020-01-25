var fs = require('fs');
var archiver = require('archiver');

var fileName =   'test.zip'
var fileOutput = fs.createWriteStream(fileName);
const archive = archiver('zip');

fileOutput.on('close', function () {
    console.log(archive.pointer() + ' total bytes');
    console.log('archiver has been finalized and the output file descriptor has closed.');
});

archive.pipe(fileOutput);
archive.directory('dist/', false);
archive.on('error', function(err){
    throw err;
});
archive.finalize();