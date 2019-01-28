const LocalStorageManager = require("../commons/LocalStorageManager");
const WorkerDataCommand = require("../commons/WorkerDataCommand");

const SrvListItemComponent = {
    create() {
        return {
            props: ["filename"],
            data: function () {
                return {
                    editorVisible: false
                }
            },
            methods: {
                btnShowEditorClicked() {
                    this.editorVisible = true;
                    this.filenameBeforeEdit = this.filename;
                },
                btnHideEditorClicked() {
                    this.editorVisible = false;
                    if (this.filenameBeforeEdit !== this.filename) {
                        window.fs.renameSync(window.path.join(LocalStorageManager.getDestDir(), this.filenameBeforeEdit), window.path.join(LocalStorageManager.getDestDir(), this.filename));
                    }
                },
                btnShowInFilesClicked() {
                    if (this.exportedFilePath) {
                        window.electron.shell.showItemInFolder(this.exportedFilePath);
                    } else {
                        window.electron.shell.showItemInFolder(window.path.join(LocalStorageManager.getDestDir(), this.filename));
                    }
                },
                btnExportClicked() {
                    this.exportedFilePath = window.electron.remote.dialog.showSaveDialog(window.electron.remote.getCurrentWindow(), {
                        defaultPath: `${window.path.basename(this.filename, ".srv")}.mp4`,
                        title: "选择导出文件路径"
                    });
                    if (this.exportedFilePath) {
                        this.working = true;
                        let worker = new Worker("../commons/workers/SrvReader/SrvReaderWorker.js");
                        worker.onmessage = e => {
                            switch (e.data.cmd) {
                                case WorkerDataCommand.CMD_READY:
                                    worker.postMessage(WorkerDataCommand.makeRequestEncodeSrvToWebmCommand(window.path.join(LocalStorageManager.getDestDir(), this.filename), this.exportedFilePath));
                                    break;
                            }
                        };
                    }
                }
            },
            template: `
<div class="srv-file-list-item">
    <div class="file-name-label" style="left: 0;top: 0;right: 0;bottom: 0;position: absolute;display: flex;flex-direction: row;align-items: center;">
        <div style="width: 5px"></div><div>{{filename}}</div>
    </div>
    <div class="file-name-editor" v-show="editorVisible" style="background-color: rgba(255,255,255,0.6);z-index:1001;position: absolute;left: 0;top: 0;right: 0;bottom: 0;display: flex;flex-direction: row;align-items: center">
        <div style="width: 5px"></div>
        <input type="text" v-model="filename" style="width: 300px;">
        <div class="btn-group" role="group">
            <a href="#" class="btn btn-outline-dark btn-sm" v-on:click="btnHideEditorClicked"><i class="fa fa-check"></i></a>
        </div>
    </div>
    <div class="file-operate-btns-container">
        <div style="flex: 1"></div>
            <div class="btn-group" role="group">
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnExportClicked"><i class="fa fa-file-export"></i></a>
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnShowEditorClicked"><i class="fa fa-edit"></i></a>
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnShowInFilesClicked"><i class="fa fa-eye"></i></a>
            </div>
        <div style="width: 5px;"></div>
    </div>
</div>`
        };
    }
};

module.exports = SrvListItemComponent;