module.exports = {
    mkdirs(dirPath) {
        let parent = window.path.dirname(dirPath);
        if (!window.fs.existsSync(parent)) {
            this.mkdirs(parent);
        }
        if (!window.fs.existsSync(dirPath)) {
            window.fs.mkdirSync(dirPath);
        }
    },
    formatTimeString(time) {
        return (time >= 10 ? "" : "0") + time
    }
};