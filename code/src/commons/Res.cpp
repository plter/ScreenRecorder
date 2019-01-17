//
// Created by yunp on 2018/4/21.
//

#include "Res.h"
#include "node_globals.h"


using namespace emscripten;

sr::Res *sr::Res::__instance = nullptr;

sr::Res *sr::Res::getInstance() {
    if (!__instance) {
        __instance = new Res();
    }
    return __instance;
}

emscripten::val &sr::Res::getDefaultFontFilePath() {
    if (fontFilePath.isNull() || fontFilePath.isUndefined()) {
        fontFilePath = NODE_PATH_JOIN(getFontsRoot(), defaultFontFileName);
    }
    return fontFilePath;
}

sr::Res::Res() :
        fontFilePath(val::undefined()),
        defaultFontFileName(val("ArialBlack.ttf")),
        resRoot(val::undefined()),
        fontsRoot(val::undefined()) {

}

const val &sr::Res::getDefaultFontFileName() const {
    return defaultFontFileName;
}

val &sr::Res::getResRoot() {
    if (resRoot.isUndefined() || resRoot.isNull()) {
        resRoot = NODE_PATH_JOIN(APP_ROOT, val("res"));
    }
    return resRoot;
}

val &sr::Res::getFontsRoot() {
    if (fontsRoot.isUndefined() || fontsRoot.isNull()) {
        fontsRoot = NODE_PATH_JOIN(getResRoot(), val("fonts"));
    }
    return fontsRoot;
}
