//
// Created by yunp on 2018/4/9.
//

#ifndef SCREENRECORDER_STREAMQUEUE_H
#define SCREENRECORDER_STREAMQUEUE_H

#include <emscripten/bind.h>
#include <queue>

namespace sr {
    class StreamQueue {
    private:
        emscripten::val _targetPath;
        std::queue<emscripten::val> _blobQueue;
        bool _idle;
        emscripten::val _fileReader;
    public:
        StreamQueue();

        void appendData(emscripten::val data);

        void checkToStartWrite();

        void popAndWrite();

        void fileReaderOnLoadHandler();
    };
}

#endif //SCREENRECORDER_STREAMQUEUE_H
