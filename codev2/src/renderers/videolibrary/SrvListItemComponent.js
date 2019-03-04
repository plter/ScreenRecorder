const LocalStorageManager = require("../commons/LocalStorageManager");

const SrvListItemComponent = {
    create() {
        return {
            props: ["filename", "file"],
            data: function () {
                return this.file.boundData;
            },
            methods: {
                btnShowEditorClicked() {
                    if (this.working) {
                        alert("正在工作中，不能重命名！");
                        return;
                    }

                    this.file.editorVisible = true;
                    this.filenameBeforeEdit = this.filename;
                },

                btnHideEditorClicked() {
                    this.file.editorVisible = false;
                    if (this.filenameBeforeEdit !== this.filename) {
                        window.fs.renameSync(window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filenameBeforeEdit), window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filename));
                        this.file.filename = this.filename;
                    }
                },

                btnShowInFilesClicked() {
                    this.file.btnShowInFilesClicked();
                },
                btnExportClicked() {
                    this.file.btnExportClicked();
                },
                btnDeleteFileClicked() {
                    if (this.working) {
                        alert("正在工作中，不可删除！");
                        return;
                    }
                    if (confirm("您真的要删除该文件吗？")) {
                        this.$emit(SrvListItemComponent.EVENT_DELETE_FILE, this.filename);
                    }
                },
                startFfmpegWorker(inWebmFile, outMp4File, videoDuration) {
                    this.file.startFfmpegWorker(inWebmFile, outMp4File, videoDuration);
                }
            },
            template: `
<div class="srv-file-list-item">
    <div style="position: absolute;left: 0;top: 0;right: 0;bottom: 0;">
        <div style="background-color: darkseagreen;height: 100%;" v-bind:style="{width:exportProgress+'%'}"></div>
    </div>
    <div class="file-name-label" style="left: 0;top: 0;right: 0;bottom: 0;position: absolute;display: flex;flex-direction: row;align-items: center;">
        <div style="width: 5px"></div>
        <div>{{filename}}</div>
        <div style="width: 5px"></div>
        <div class="alert alert-dark" style="margin: 0;font-size: 10pt;padding: 2px;border: 0;" v-if="state">{{state}}</div>
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
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnDeleteFileClicked"><i class="fa fa-trash"></i></a>
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnShowInFilesClicked"><i class="fa fa-eye"></i></a>
            </div>
        <div style="width: 5px;"></div>
    </div>
</div>`
        };
    },
    EVENT_DELETE_FILE: "delete-file",
    EVENT_WORKING: "working",
    EVENT_IDLE: "idle"
};

module.exports = SrvListItemComponent;