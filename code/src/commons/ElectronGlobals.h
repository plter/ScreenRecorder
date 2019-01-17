//
// Created by yunp on 2018/5/19.
//

#ifndef SCREENRECORDER_ELECTRONGLOBALS_H
#define SCREENRECORDER_ELECTRONGLOBALS_H

#include <emscripten/bind.h>


#define ELECTRON sr::ElectronGlobals::getInstance()->get_electron()
#define ELECTRON_DESKTOP_CAPTURER sr::ElectronGlobals::getInstance()->get_desktopCapturer()

namespace sr {
    class ElectronGlobals {

    private:
        static ElectronGlobals *instance;

    private:
        emscripten::val _electron;
        emscripten::val _desktopCapturer;

    public:
        ElectronGlobals();

        emscripten::val getAppVersion();

        const emscripten::val &get_electron() const;

        const emscripten::val &get_desktopCapturer() const;

        static ElectronGlobals *getInstance();
    };
}


#endif //SCREENRECORDER_ELECTRONGLOBALS_H
