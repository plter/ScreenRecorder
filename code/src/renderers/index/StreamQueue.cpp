//
// Created by yunp on 2018/4/9.
//

#include "StreamQueue.h"
#include "../../commons/node_globals.h"
#include "../../commons/LocalStorageManager.h"
#include <boost/format.hpp>
#include <iostream>
#include <emscripten.h>
#include "../../commons/SRGlobals.h"

using namespace emscripten;

namespace sr {
    StreamQueue::StreamQueue() :
            _targetPath(val::undefined()),
            _idle(true),
            _fileReader(val::global("FileReader").new_()) {

        size_t bufLength = 30;
        time_t rawTime;
        struct tm *timeinfo;
        time(&rawTime);
        timeinfo = localtime(&rawTime);
        char timeStrBuf[bufLength];
        memset(timeStrBuf, 0, bufLength);
        strftime(timeStrBuf, bufLength, "%Y%m%d%H%M%S", timeinfo);

        val destDir = LocalStorageManager::getSRLibraryDirectory();

        if (!NODE_FS.call<val>("existsSync", destDir).as<bool>()) {
            NODE_FS.call<void>("mkdirSync", destDir);
        }
        _targetPath = NODE_PATH_JOIN(LocalStorageManager::getSRLibraryDirectory(),
                                     (boost::format("%1%%2%") % std::string(timeStrBuf) % SRV_FILE_TYPE).str());

        _fileReader.set("onload", JS_FUNCTION(
                (boost::format("Module.Index_StreamQueue_fileReader_onload(%1%)") % (unsigned long) this).str()));

    }

    EMSCRIPTEN_BINDINGS(Index_StreamQueue) {
        function("Index_StreamQueue_fileReader_onload", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
            reinterpret_cast<StreamQueue *>(__this)->fileReaderOnLoadHandler();
        });
    }

    void StreamQueue::appendData(emscripten::val data) {
        _blobQueue.push(data);

        checkToStartWrite();
    }

    void StreamQueue::checkToStartWrite() {
        if (_idle) {
            popAndWrite();
            _idle = false;
        }
    }

    void StreamQueue::popAndWrite() {
        val first = _blobQueue.front();
        _blobQueue.pop();
        _fileReader.call<void>("readAsArrayBuffer", first);
    }


    void StreamQueue::fileReaderOnLoadHandler() {
        val blobDataArrayBuffer = _fileReader["result"];
        val blobDataArrayBufferSize = blobDataArrayBuffer["byteLength"];

        //make a buffer to store content data type
        val typeBuffer = val::global("Buffer").call<val>("alloc", val(1));
        typeBuffer.call<void>("writeUInt8", val(1));//type number 1 is for webm data part
        NODE_FS.call<void>("appendFileSync", _targetPath, typeBuffer);//write data type to file

        //make a buffer to store content data size
        val sizeBuffer = val::global("Buffer").call<val>("alloc", val(4));
        sizeBuffer.call<void>("writeUInt32LE", blobDataArrayBufferSize);
        NODE_FS.call<void>("appendFileSync", _targetPath, sizeBuffer);//Write content size

        //write content
        NODE_FS.call<void>("appendFileSync", _targetPath,
                           val::global("Buffer").call<val>("from", blobDataArrayBuffer));

        if (!_blobQueue.empty()) {
            popAndWrite();
        } else {
            _idle = true;
        }
    }
}