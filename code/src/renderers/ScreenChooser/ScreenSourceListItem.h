//
// Created by yunp on 2018/4/8.
//

#ifndef SCREENRECORDER_SCREENSOURCELISTITEM_H
#define SCREENRECORDER_SCREENSOURCELISTITEM_H


#include <emscripten/bind.h>

namespace sr {
    class ScreenSourceListItem {

    private:
        emscripten::val _screenSource;
        emscripten::val _node;
    public:
        const emscripten::val &get_node() const;

    private:
        emscripten::val _jq;
    public:
        explicit ScreenSourceListItem(const emscripten::val &_screenSource);

        void buildUI();

        void addListeners();

        void onClickedHandler();
    };
}

#endif //SCREENRECORDER_SCREENSOURCELISTITEM_H
