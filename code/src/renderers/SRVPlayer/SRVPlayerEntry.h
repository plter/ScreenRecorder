//
// Created by yunp on 2018/4/18.
//

#pragma once

#include <emscripten/bind.h>

namespace sr {
    class SRVPlayerEntry {

    private:
        emscripten::val _videoPlayer;
        emscripten::val _seekBar;
        emscripten::val _seekBarJq;
        emscripten::val statusDiv;
    public:
        SRVPlayerEntry();

    private:
        static SRVPlayerEntry *__instance;

    public:
        static SRVPlayerEntry *getInstance();

        void playVideo();

        void addListeners();

        void playerTimeUpdateHandler();

        void onSlideHandler(unsigned int i);

        void pauseCurrentVideo();

        void playCurrentVideo();
    };
}
