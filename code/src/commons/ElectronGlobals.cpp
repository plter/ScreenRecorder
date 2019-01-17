//
// Created by yunp on 2018/5/19.
//

#include "ElectronGlobals.h"

using namespace emscripten;

namespace sr {

    ElectronGlobals *ElectronGlobals::instance = nullptr;

    ElectronGlobals::ElectronGlobals() :
            _electron(val::global("require")(val("electron"))),
            _desktopCapturer(val::global("require")(val("electron"))["desktopCapturer"]) {

    }

    const val &ElectronGlobals::get_electron() const {
        return _electron;
    }

    const val &ElectronGlobals::get_desktopCapturer() const {
        return _desktopCapturer;
    }

    ElectronGlobals *ElectronGlobals::getInstance() {
        if (!instance) {
            instance = new ElectronGlobals();
        }
        return instance;
    }

    emscripten::val ElectronGlobals::getAppVersion() {
        return get_electron()["remote"]["app"].call<val>("getVersion");
    }
}
