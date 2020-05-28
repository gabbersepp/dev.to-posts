const CDP = require('chrome-remote-interface')
const {execFile} = require("child_process")

function ensureRdpPort(args) {
  args = args.args || args;
  const existing = args.find((arg) => arg.slice(0, 23) === '--remote-debugging-port')

  if (existing) {
    return Number(existing.split('=')[1])
  }

  const port = 40000 + Math.round(Math.random() * 25000)

  args.push(`--remote-debugging-port=${port}`)

  return port
}

let port = 0
let client = null

module.exports = (on, config) => {
  on('before:browser:launch', (browser, launchOptionsOrArgs) => {
    const args = Array.isArray(launchOptionsOrArgs) ? launchOptionsOrArgs : launchOptionsOrArgs.args
    console.log(JSON.stringify(args));
    port = ensureRdpPort(args)
    console.log(port);
  })

  on('task', {
    nativeClick: async({ x, y }) => {
      client = client || await CDP({ port })
      await client.Input.dispatchMouseEvent( { type: "mousePressed", x, y, button: 'left', clickCount: 1 });
      await client.Input.dispatchMouseEvent( { type: "mouseReleased", x, y, button: 'left', clickCount: 1, buttons: 1 });
      return Promise.resolve(true);
    },
    selectFile: async(value) => {
      return new Promise(resolve => {
        execFile("C:/git/dev.to-posts/blog-posts/cypress-file-chooser/code/tool/Tool/Tool/bin/Debug/Tool.exe", [value], {}, (error) => {
          resolve("ready" + JSON.stringify(error));
        })
      })
    },
    resetCRI: async () => {
      if (client) {
        await client.close()
        client = null
      }

      return Promise.resolve(true)
    }
  })
}
