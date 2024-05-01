module.exports = (on) => {
    //...
    on("task", {
        //...
        resetCRI: async () => {
            if (client) {
                await client.close();
            }

            return Promise.resolve(true);
        }
    });
    //...
}
