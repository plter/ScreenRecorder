//
// Created by yunp on 2018/5/15.
//

#ifndef SCREENRECORDER_SRVREADERTASK_H
#define SCREENRECORDER_SRVREADERTASK_H


#include <emscripten/bind.h>
#include <functional>
#include <string>

namespace sr {
    class SRVReaderTask {

    private:
        emscripten::val work;
        std::function<void(std::string type, std::wstring message, unsigned long current,
                           unsigned long total, unsigned long srvDuration)> progressHandler;
        std::function<void(void)> readyHandler;

    public:
        SRVReaderTask();

        void mountNODEFS();

        void encodeSrvToWebm(std::string inFileNodeFSPath, std::string outFileNodeFSPath);

        const std::function<void(std::string, std::wstring, unsigned long, unsigned long, unsigned long srvDuration)> &
        getProgressHandler() const;

        void setProgressHandler(
                const std::function<void(std::string, std::wstring, unsigned long, unsigned long,
                                         unsigned long srvDuration)> &progressHandler);

        void onMessageHandler(emscripten::val event);

        const std::function<void()> &getReadyHandler() const;

        void setReadyHandler(const std::function<void()> &readyHandler);

        void terminate();
    };
}


#endif //SCREENRECORDER_SRVREADERTASK_H
