//
// Created by yunp on 2018/4/9.
//

#ifndef SCREENRECORDER_VIDEOLIBRARYENTRY_H
#define SCREENRECORDER_VIDEOLIBRARYENTRY_H

#include <emscripten/bind.h>
#include "VideoFileListItem.h"
#include <vector>

namespace sr {
    class VideoLibraryEntry {
    private:
        static VideoLibraryEntry *__instance;
        emscripten::val _panelBody;
        std::vector<std::shared_ptr<VideoFileListItem>> _items;

    public:
        static VideoLibraryEntry *getInstance();

        VideoLibraryEntry();

        void buildUI();
    };
}


#endif //SCREENRECORDER_VIDEOLIBRARYENTRY_H
