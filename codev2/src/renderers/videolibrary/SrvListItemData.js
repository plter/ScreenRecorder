const LocalStorageManager = require("../commons/LocalStorageManager");
const WorkerDataCommand = require("../commons/WorkerDataCommand");
const Tools = require("../commons/Tools");

class SrvListItemData {

    constructor(filename) {

        SrvListItemData.__id = SrvListItemData.__id || 1;
        SrvListItemData.__id++;

        this.filename = filename;
        this.id = SrvListItemData.__id;

        this.boundData = {
            editorVisible: false,
            working: false,
            exportProgress: 0,
            state: ''
        };
    }

    get working() {
        return this.boundData.working;
    }

    set working(value) {
        this.boundData.working = value;
    }

    get editorVisible() {
        return this.boundData.editorVisible;
    }

    set editorVisible(value) {
        this.boundData.editorVisible = value;
    }

    get state() {
        return this.boundData.state;
    }

    set state(value) {
        this.boundData.state = value;
    }

    get exportProgress() {
        return this.boundData.exportProgress;
    }

    set exportProgress(value) {
        this.boundData.exportProgress = value;
    }


    btnShowEditorClicked() {
        if (this.working) {
            alert("正在工作中，不能重命名！");
            return;
        }

        this.editorVisible = true;
        this.filenameBeforeEdit = this.filename;
    }

    btnHideEditorClicked() {
        this.editorVisible = false;
        if (this.filenameBeforeEdit !== this.filename) {
            window.fs.renameSync(window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filenameBeforeEdit), window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filename));
        }
    }

    btnShowInFilesClicked() {
        if (this.exportedFilePath) {
            window.electron.shell.showItemInFolder(this.exportedFilePath);
        } else {
            window.electron.shell.showItemInFolder(window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filename));
        }
    }

    btnExportClicked() {
        if (this.working) {
            alert("正在工作中，请不要重复执行导出任务！");
            return;
        }

        let inSrvFile = window.path.join(LocalStorageManager.getVideoLibraryDir(), this.filename);

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
    }

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
}

module.exports = SrvListItemData;