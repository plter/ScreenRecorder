//
// Created by yunp on 2018/5/13.
//

#include <emscripten.h>
#include <emscripten/bind.h>
#include "../../commons/node_globals.h"
#include "../../commons/SRGlobals.h"
#include "../../commons/worker/SRVReader.h"
#include <string>
#include <iostream>
#include <memory>
#include "../../commons/worker/WorkerGlobals.h"

using namespace emscripten;
using namespace sr;

void addListeners() {
    val::global("self").set("onmessage", sr::SRGlobals::JSFunction1WB("event", LR"(
  Module.onMessageHandler(event);
)"));
}

void encodeSrvToWebm(std::string inFileNodeFSPath, std::string outFileNodeFSPath) {
    auto reader = std::make_shared<SRVReader>(inFileNodeFSPath);
    reader->read();
    unsigned long srvDuration = 0;
    if (reader->hasSRVWebM()) {
        srvDuration = reader->getFirstSRVWebM()->getSrvDuration();
        reader->getFirstSRVWebM()->writeTo(outFileNodeFSPath);
    }
    WorkerGlobals::postProgress("complete", L"转码完成", 1, 1, srvDuration);
}

EMSCRIPTEN_BINDINGS(SrvReaderTaskImpl) {
    function("onMessageHandler", LAMBDA_TO_FUNCTION_POINTER(val event) {
        auto command = event["data"]["command"].as<std::string>();
        if (command == "mountNODEFS") {
            NodeGlobals::getInstance()->mountNODEFS(event["data"]["srLibraryDirectory"]);
            CONSOLE_LOG(val("Success to mount NodeFS"));
        } else if (command == "encodeSrvToWebm") {
            CONSOLE_LOG(val("Worker received command encodeSrvToWebm"));
            encodeSrvToWebm(event["data"]["inFileNodeFSPath"].as<std::string>(),
                            event["data"]["outFileNodeFSPath"].as<std::string>());
        }
    });
}

int main() {
    addListeners();
    val::global("self").call<void>("postMessage", MAKE_JSON_OBJECT(R"({"type":"ready"})"));
    return 0;
}