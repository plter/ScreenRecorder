//
// Created by yunp on 2018/4/21.
//

#ifndef SCREENRECORDER_RES_H
#define SCREENRECORDER_RES_H

#include <emscripten/val.h>

namespace sr {

    class Res {
    private:
        static Res *__instance;

    private:
        emscripten::val fontFilePath;
        emscripten::val defaultFontFileName;
        emscripten::val resRoot;
        emscripten::val fontsRoot;

    public:
        static Res *getInstance();

    public:
        Res();

        emscripten::val &getDefaultFontFilePath();

        const emscripten::val &getDefaultFontFileName() const;

        emscripten::val &getResRoot();

        emscripten::val &getFontsRoot();
    };
}


#endif //SCREENRECORDER_RES_H
