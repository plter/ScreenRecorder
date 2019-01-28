const LocalStorageManager = require("../commons/LocalStorageManager");


const SrvListConfig = {
    create(el) {
        return {
            el: el,
            data: {
                srvFiles: window.fs.readdirSync(LocalStorageManager.getDestDir()).sort().reverse().filter(
                    value => {
                        return value.toLowerCase().endsWith(".srv");
                    }
                )
            }
        }
    }
};

module.exports = SrvListConfig;