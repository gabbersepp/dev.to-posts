const FtpClient = require('ftp-client');

async function upload() {
    const config = {
        host: process.env.FTP_SERVER,
        user: process.env.FTP_USER,
        password: process.env.FTP_PWD,
        port: 21,
        secure: true
    };

    const options = {
        logging: 'basic'
    };

    const client = new FtpClient(config, options);

    client.connect(function () {
        client.upload(['dist/**'], '/', {
            overwrite: 'older'
        }, function (result) {
            console.log(result);
        });
    });
  }

  upload()
