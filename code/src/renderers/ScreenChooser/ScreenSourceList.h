//
// Created by yunp on 2018/4/8.
//

#ifndef SCREENRECORDER_SCREENSOURCELIST_H
#define SCREENRECORDER_SCREENSOURCELIST_H


#include <emscripten/bind.h>
#include "ScreenSourceListItem.h"
#include <vector>
#include <memory>

namespace sr {

    class ScreenSourceList {
    private:
        emscripten::val _sources;
        emscripten::val _container;
        std::vector<std::shared_ptr<ScreenSourceListItem>> _items;
    public:
        explicit ScreenSourceList(emscripten::val _sources);

        void initUI();
    };
}


#endif //SCREENRECORDER_SCREENSOURCELIST_H
