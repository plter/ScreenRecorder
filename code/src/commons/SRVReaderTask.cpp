//
// Created by yunp on 2018/5/15.
//

#include "SRVReaderTask.h"
#include "SRGlobals.h"
#include "node_globals.h"
#include "LocalStorageManager.h"
#include <boost/format.hpp>
#include <emscripten/bind.h>

using namespace emscripten;

namespace sr {

    EMSCRIPTEN_BINDINGS(SRVReaderTask) {
        function("SRVReaderTaskOnMessageHandler", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, val event) {
            reinterpret_cast<SRVReaderTask *>(__this)->onMessageHandler(event);
        });
    };


    SRVReaderTask::SRVReaderTask() : work(val::undefined()) {
        work = SRGlobals::JavaScriptObjectNew1(val::global("Worker"), val("SrvReaderTaskImpl.js"));
        work.set("onmessage", SRGlobals::JSFunction1WB("event", (boost::wformat(LR"(
Module.SRVReaderTaskOnMessageHandler(%1%,event);
)") % (unsigned long) this).str()));
        printf("Create work\n");
    }

    void SRVReaderTask::mountNODEFS() {
        val obj = val::object();
        obj.set("command", val("mountNODEFS"));
        obj.set("srLibraryDirectory", LocalStorageManager::getSRLibraryDirectory());
        printf("Request worker mountNODEFS\n");
        work.call<void>("postMessage", obj);
    }

    void SRVReaderTask::encodeSrvToWebm(std::string inFileNodeFSPath, std::string outFileNodeFSPath) {
        val obj = val::object();
        obj.set("command", val("encodeSrvToWebm"));
        obj.set("inFileNodeFSPath", val(inFileNodeFSPath));
        obj.set("outFileNodeFSPath", val(outFileNodeFSPath));
        work.call<void>("postMessage", obj);
    }

    const std::function<void(std::string, std::wstring, unsigned long, unsigned long, unsigned long srvDuration)> &
    SRVReaderTask::getProgressHandler() const {
        return progressHandler;
    }

    void SRVReaderTask::setProgressHandler(
            const std::function<void(std::string, std::wstring, unsigned long, unsigned long,
                                     unsigned long srvDuration)> &progressHandler) {
        SRVReaderTask::progressHandler = progressHandler;
    }

    void SRVReaderTask::onMessageHandler(emscripten::val event) {
        val data = event["data"];
        if (!data.isUndefined() && !data.isNull()) {
            val messageType = data["type"];
            if (messageType.equals(val("progress"))) {
                if (progressHandler) {
                    progressHandler(data["state"].as<std::string>(), data["message"].as<std::wstring>(),
                                    data["current"].as<unsigned long>(), data["total"].as<unsigned long>(),
                                    data["srvDuration"].as<unsigned long>());
                }
            } else if (messageType.equals(val("ready"))) {
                if (getReadyHandler()) {
                    getReadyHandler()();
                }
            }
        }
    }

    void SRVReaderTask::terminate() {
        work.call<void>("terminate");
    }

    const std::function<void()> &SRVReaderTask::getReadyHandler() const {
        return readyHandler;
    }

    void SRVReaderTask::setReadyHandler(const std::function<void()> &readyHandler) {
        SRVReaderTask::readyHandler = readyHandler;
    }
}
