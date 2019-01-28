/**
 * Created by plter on 3/24/17.
 */

class StreamQueue {

    constructor(distFile) {
        this._distFile = distFile;
        this._idle = true;
        this._blobs = [];
        this._fileReader = new FileReader();
        this._fileReader.onload = () => this.fileLoadedHandler();
    }

    get distFile() {
        return this._distFile;
    }

    appendData(blob, timeStamp) {
        this._blobs.push({data: blob, time: timeStamp});
        this.checkToStartWrite();
    }

    checkToStartWrite() {
        if (this._idle) {
            this.shiftAndWrite();
            this._idle = false;
        }
    }

    shiftAndWrite() {
        this._currentBlob = this._blobs.shift();
        this._fileReader.readAsArrayBuffer(this._currentBlob.data);
    }

    fileLoadedHandler() {
        let dataArrayBuffer = this._fileReader.result;

        let sizeBuffer = Buffer.alloc(4);
        sizeBuffer.writeUInt32LE(dataArrayBuffer.byteLength, 0);

        let timeBuffer = Buffer.alloc(4);
        let time = Math.floor(this._currentBlob.time);
        timeBuffer.writeUInt32LE(time, 0);

        window.fs.appendFileSync(this.distFile, timeBuffer);
        window.fs.appendFileSync(this.distFile, sizeBuffer);
        window.fs.appendFileSync(this.distFile, Buffer.from(dataArrayBuffer));

        if (this._blobs.length > 0) {
            this.shiftAndWrite();
        } else {
            this._idle = true;
        }
    }
}

module.exports = StreamQueue;