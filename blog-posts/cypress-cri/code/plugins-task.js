let client;

module.exports = (on) => {
    //...
    on("task", {
        activatePrintMediaQuery: async () => {
            client = client || await CDP({ port });
            return client.send('Emulation.setEmulatedMedia', { media: "print" })
        }
    })
    //...
}