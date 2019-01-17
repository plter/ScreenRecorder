//
// Created by yunp on 2018/4/8.
//

#ifndef SCREENRECORDER_SCREENCHOOSERENTRY_H
#define SCREENRECORDER_SCREENCHOOSERENTRY_H

#include <emscripten/bind.h>
#include "ScreenSourceList.h"

namespace sr {
    class ScreenChooserEntry {
    private:
        static ScreenChooserEntry *__instance;

        emscripten::val _container;
        ScreenSourceList *_screenSourceList;

    public:
        ScreenChooserEntry();

        virtual ~ScreenChooserEntry();

    public:
        static ScreenChooserEntry *getInstance();

        void searchStreams();

        void onGotScreenStreamSourcesHandler(emscripten::val sources);
    };
}

#endif //SCREENRECORDER_SCREENCHOOSERENTRY_H
