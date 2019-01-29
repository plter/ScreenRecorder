let WorkerDataCommand = require("../../WorkerDataCommand");
let fs = require("fs");


function readBlobInfo(fd, position) {
    let timeBuffer = Buffer.alloc(4);
    fs.readSync(fd, timeBuffer, 0, 4, position);
    position += 4;
    let time = timeBuffer.readUInt32LE(0);

    let dataLengthBuffer = Buffer.alloc(4);
    fs.readSync(fd, dataLengthBuffer, 0, 4, position);
    position += 4;
    let dataLength = dataLengthBuffer.readUInt32LE(0);

    return {time: time, dataLength: dataLength};
}

function encodeSrvToWebm(ev) {
    postMessage(WorkerDataCommand.makeReadingSrvFileCommand());
    let inSrvFileFd = fs.openSync(ev.data.inSrvPath, "r");
    let inSrvFileStat = fs.statSync(ev.data.inSrvPath);
    let blobs = [];

    let position = 0;
    //read first blob
    let blobInfo = readBlobInfo(inSrvFileFd, position);
    blobs.push(blobInfo);
    position += 8;
    position += blobInfo.dataLength;

    while (position < inSrvFileStat.size) {
        let blobInfo = readBlobInfo(inSrvFileFd, position);
        blobs.push(blobInfo);
        position += 8;
        position += blobInfo.dataLength;
        postMessage(WorkerDataCommand.makeFoundBlobCommand(blobs.length));
    }

    let blobsCount = blobs.length;
    position = 0;
    //read data to webm file
    for (let i = 0; i < blobsCount; i++) {
        position += 8;
        let blobInfo = blobs[i];
        let buffer = Buffer.alloc(blobInfo.dataLength);
        fs.readSync(inSrvFileFd, buffer, 0, blobInfo.dataLength, position);
        fs.appendFileSync(ev.data.outWebmPath, buffer);
        position += blobInfo.dataLength;
        postMessage(WorkerDataCommand.makeConvertSrvToWebmProgress(i + 1, blobs.length));
    }
    fs.closeSync(inSrvFileFd);
    postMessage(WorkerDataCommand.makeConvertSrvToWebComplete(blobs, ev.data.outWebmPath));
}

onmessage = ev => {
    switch (ev.data.cmd) {
        case WorkerDataCommand.CMD_REQUEST_CONVERT_SRV_TO_WEBM:
            encodeSrvToWebm(ev);
            break;
    }
};


postMessage(WorkerDataCommand.makeReadyCommand());