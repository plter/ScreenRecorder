//
// Created by yunp on 2018/4/10.
//

#ifndef SCREENRECORDER_SRWINDOWS_H
#define SCREENRECORDER_SRWINDOWS_H

#include <emscripten/bind.h>

namespace sr {
    class SRWindows {
    public:
        static void showVideoLibraryWindow();

        static void showSRVPlayerWindow();
        static void showAboutWindow();
        static void showSettingsWindow();
        static emscripten::val showChooseScreenStreamWindow();
        static emscripten::val showPendingToStartWindows();

        static emscripten::val showWindow(emscripten::val &path, emscripten::val &windowOptions);
    };
}

#endif //SCREENRECORDER_SRWINDOWS_H
