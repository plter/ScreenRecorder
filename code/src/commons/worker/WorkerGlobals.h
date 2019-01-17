//
// Created by yunp on 2018/5/20.
//

#ifndef SCREENRECORDER_WORKERGLOBALS_H
#define SCREENRECORDER_WORKERGLOBALS_H

#include <emscripten/val.h>
#include <string>
#include <stdint.h>

namespace sr {

    enum RuntimeEnv {
        Unknown,
        Worker,
        MainThread
    };

    class WorkerGlobals {

    private:
        static RuntimeEnv env;

    public:
        static void postMessage(emscripten::val data);

        static void
        postProgress(std::string state, std::wstring msg, unsigned long currentPosition, unsigned long total,
                     unsigned long srvDuration = 0);

        static RuntimeEnv getEnv();
    };
}

#endif //SCREENRECORDER_WORKERGLOBALS_H
