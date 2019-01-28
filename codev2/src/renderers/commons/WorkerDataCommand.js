const WorkerDataCommand = {
    CMD_READY: "ready",
    CMD_PROGRESS: "progress",
    CMD_COMPLETE: "complete",
    CMD_REQUEST_ENCODE_SRV_TO_WEBM: "requestEncodeSrvToWebm",
    CMD_READING_SRV_FILE: "readingSrvFile",
    makeReadyCommand() {
        return {cmd: this.CMD_READY};
    },
    makeProgressCommand(progress) {
        return {cmd: this.CMD_PROGRESS, progress: progress}
    },
    makeCompleteCommand() {
        return {cmd: this.CMD_COMPLETE}
    },
    makeRequestEncodeSrvToWebmCommand(inSrvPath, outWebmPath) {
        return {cmd: this.CMD_REQUEST_ENCODE_SRV_TO_WEBM, inSrvPath: inSrvPath, outWebmPath: outWebmPath};
    },
    makeReadingSrvFileCommand() {
        return {cmd: this.CMD_READING_SRV_FILE};
    }
};

module.exports = WorkerDataCommand;