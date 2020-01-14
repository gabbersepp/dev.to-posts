const ftp = require("basic-ftp")
 
async function upload() {
    const client = new ftp.Client();
    client.ftp.verbose = true;

    try {
        await client.access({
            host: process.env.FTP_SERVER,
            user: process.env.FTP_USER,
            password: process.env.FTP_PWD,
            secure: true
        });
        await client.uploadFromDir("./page/dist", "./");
    } catch(err) {
        console.log(err);
    }

    client.close();
}

upload()