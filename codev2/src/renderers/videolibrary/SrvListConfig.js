const LocalStorageManager = require("../commons/LocalStorageManager");
const SrvListItemData = require("./SrvListItemData");


const SrvListConfig = {
    create(el) {
        let fileWrappers = [];

        let files = window.fs.readdirSync(LocalStorageManager.getVideoLibraryDir()).sort().reverse().filter(
            value => {
                return value.toLowerCase().endsWith(".srv");
            }
        ).forEach(value => {
            fileWrappers.push(new SrvListItemData(value));
        });


        return {
            el: el,
            data: {
                srvFiles: fileWrappers
            },
            methods: {
                onDeleteFile: function (file) {
                    let filepath = window.path.join(LocalStorageManager.getVideoLibraryDir(), file);
                    for (let i = 0; i < this.srvFiles.length; i++) {
                        if (this.srvFiles[i].filename == file) {
                            this.srvFiles.splice(i, 1);
                            break;
                        }
                    }
                    window.electron.shell.moveItemToTrash(filepath);
                }
            }
        }
    }
};

module.exports = SrvListConfig;