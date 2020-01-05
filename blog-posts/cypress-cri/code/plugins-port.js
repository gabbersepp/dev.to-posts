const CDP = require('chrome-remote-interface');

let port = 0;

module.exports = (on) => {
  on('before:browser:launch', (browser, args) => {
    port = ensureRdpPort(args);
  })
}

function ensureRdpPort(args) {
  const existing = args.find(arg => arg.slice(0, 23) === '--remote-debugging-port')

  if (existing) {
    return Number(existing.split('=')[1])
  }

  port = 40000 + Math.round(Math.random() * 25000)
  args.push(`--remote-debugging-port=${port}`)
  return port
}