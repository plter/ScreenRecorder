let WorkerDataCommand = require("../../WorkerDataCommand");
let fs = require("fs");


function encodeSrvToWebm(ev) {
    postMessage(WorkerDataCommand.makeReadingSrvFileCommand());
    let inSrvFileFd = fs.openSync(ev.data.inSrvPath, "r");
    let inSrvFileStat = fs.statSync(ev.data.inSrvPath);
    let webmPartsCount = 0;
    let webmDuration = 0;

    let position = 0;
    //read first blob
    let timeBuffer = Buffer.alloc(4);
    fs.readSync(inSrvFileFd, timeBuffer, 0, 4, position);
    let firstBlobTime = timeBuffer.readUInt32LE(0);
    position += 4;
    let dataLengthBuffer = Buffer.alloc(4);
    fs.readSync(inSrvFileFd, dataLengthBuffer, 0, 4, position);
    position += 4;
    let dataLength = dataLengthBuffer.readUInt32LE(0);
    position += dataLength;
    webmPartsCount++;

    let lastBlobTime = firstBlobTime;
    while (position < inSrvFileStat.size) {
        let timeBuffer = Buffer.alloc(4);
        fs.readSync(inSrvFileFd, timeBuffer, 0, 4, position);
        position += 4;
        lastBlobTime = timeBuffer.readUInt32LE(0);

        let dataLengthBuffer = Buffer.alloc(4);
        fs.readSync(inSrvFileFd, dataLengthBuffer, 0, 4, position);
        position += 4;
        let dataLength = dataLengthBuffer.readUInt32LE(0);
        position += dataLength;
        webmPartsCount++;
    }
    webmDuration = lastBlobTime - firstBlobTime;
    fs.closeSync(inSrvFileFd);

    console.log(webmPartsCount, webmDuration);
}

onmessage = ev => {
    switch (ev.data.cmd) {
        case WorkerDataCommand.CMD_REQUEST_ENCODE_SRV_TO_WEBM:
            encodeSrvToWebm(ev);
            break;
    }
};


postMessage(WorkerDataCommand.makeReadyCommand());