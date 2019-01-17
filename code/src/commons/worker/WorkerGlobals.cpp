//
// Created by yunp on 2018/5/20.
//

#include "WorkerGlobals.h"

using namespace emscripten;

namespace sr {

    RuntimeEnv WorkerGlobals::env = Unknown;

    void WorkerGlobals::postMessage(emscripten::val data) {
        val importScriptFunc = val::global("importScripts");
        if (getEnv() == Worker) {
            //We are running in a worker,now we can call postMessage function
            val jsPostMessage = val::global("postMessage");

            if (!jsPostMessage.isNull() && !jsPostMessage.isUndefined()) {
                jsPostMessage(data);
            }
        }
    }

    void WorkerGlobals::postProgress(std::string state, std::wstring msg, unsigned long currentPosition,
                                     unsigned long total, unsigned long srvDuration) {
        val data = val::object();
        data.set("type", "progress");
        data.set("state", val(state));
        data.set("message", val(msg));
        data.set("current", val(currentPosition));
        data.set("total", val(total));
        data.set("srvDuration", srvDuration);
        postMessage(data);
    }

    RuntimeEnv WorkerGlobals::getEnv() {
        if (env == Unknown) {
            val importScriptFunc = val::global("importScripts");
            if (!importScriptFunc.isNull() && !importScriptFunc.isUndefined() &&
                importScriptFunc.typeOf().equals(val("function"))) {
                //We are running in a worker,now we can call postMessage function
                env = Worker;
            } else {
                env = MainThread;
            }
        }
        return env;
    }
}