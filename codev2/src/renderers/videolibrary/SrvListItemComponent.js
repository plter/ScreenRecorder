const LocalStorageManager = require("../commons/LocalStorageManager");
const WorkerDataCommand = require("../commons/WorkerDataCommand");
const Tools = require("../commons/Tools");

const SrvListItemComponent = {
    create() {
        return {
            props: ["filename"],
            data: function () {
                return {
                    editorVisible: false,
                    state: "",
                    working: false,
                    exportProgress: 0
                }
            },
            methods: {
                btnShowEditorClicked() {
                    if (this.working) {
                        alert("正在工作中，不能重命名");
                        return;
                    }

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
                    if (this.working) {
                        alert("正在工作中，请不要重复执行导出任务");
                        return;
                    }

                    let inSrvFile = window.path.join(LocalStorageManager.getDestDir(), this.filename);

                    this.exportedFilePath = window.electron.remote.dialog.showSaveDialog(window.electron.remote.getCurrentWindow(), {
                        defaultPath: `${window.path.basename(this.filename, ".srv")}.mp4`,
                        title: "选择导出文件路径"
                    });
                    if (this.exportedFilePath) {
                        if (!this.exportedFilePath.toLowerCase().endsWith(".mp4")) {
                            this.exportedFilePath += ".mp4";
                        }

                        this.working = true;
                        this.state = "正在启动任务";
                        let worker = new Worker("../commons/workers/SrvReader/SrvReaderWorker.js");
                        worker.onmessage = e => {
                            switch (e.data.cmd) {
                                case WorkerDataCommand.CMD_READY:
                                    worker.postMessage(WorkerDataCommand.makeRequestConvertSrvToWebmCommand(inSrvFile, `${this.exportedFilePath}.webm`));
                                    break;
                                case WorkerDataCommand.CMD_FOUND_BLOB:
                                    this.state = `已找到个${e.data.totalFound}数据片段`;
                                    break;
                                case WorkerDataCommand.CMD_CONVERT_SRV_TO_WEBM_PROGRESS:
                                    let percent = Math.floor(e.data.currentCount / e.data.totalCount * 100);
                                    this.exportProgress = percent;
                                    this.state = `正在生成webm ${percent}%`;
                                    break;
                                case WorkerDataCommand.CMD_CONVERT_SRV_TO_WEBM_COMPLETE:
                                    this.exportProgress = 0;
                                    this.state = `已生成webm`;
                                    this.startFfmpegWorker(e.data.webmPath, this.exportedFilePath, e.data.blobs[e.data.blobs.length - 1].time - e.data.blobs[0].time);
                                    break;
                            }
                        };
                    }
                },
                startFfmpegWorker(inWebmFile, outMp4File, videoDuration) {
                    let ffmpeg = Tools.getFfmpegExeFilePath();
                    if (!ffmpeg) {
                        this.working = false;
                        this.state = "不支持该平台，请联系开发者";
                        return;
                    }

                    if (!window.fs.existsSync(ffmpeg)) {
                        this.state = "ffmpeg文件缺失，请尝试重装该软件";
                        this.working = false;
                        return;
                    }

                    let args = ["-y", "-i", inWebmFile, "-max_muxing_queue_size", "1024", "-vcodec", "copy", "-b:a", LocalStorageManager.getAudioBps(), "-r", "30", outMp4File];
                    let process = window.childProcess.execFile(ffmpeg, args, {
                        maxBuffer: 1024 * 1024 * 200
                    });
                    process.stderr.on("data", data => {
                        let result = /time=(\d{2}):(\d{2}):(\d{2})\.(\d{2})/.exec(data);
                        if (result) {
                            let h = parseInt(result[1]) * 60 * 60 * 1000;
                            let m = parseInt(result[2]) * 60 * 1000;
                            let s = parseInt(result[3] * 1000);
                            let ms = parseInt(result[4]) * 10;
                            let ratio = (h + m + s + ms) / videoDuration;
                            if (ratio > 1) {
                                ratio = 1;
                            }
                            this.exportProgress = Math.floor(ratio * 100);
                            this.state = `正在导出 ${this.exportProgress}%`;
                        }
                    });
                    process.on("close", code => {
                        this.working = false;
                        this.exportProgress = 0;
                        if (code) {
                            this.state = "导出失败";
                        } else {
                            this.state = "导出成功";
                        }
                    });
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
                <a href="#" class="btn btn-sm btn-outline-dark" v-on:click="btnShowInFilesClicked"><i class="fa fa-eye"></i></a>
            </div>
        <div style="width: 5px;"></div>
    </div>
</div>`
        };
    }
};

module.exports = SrvListItemComponent;