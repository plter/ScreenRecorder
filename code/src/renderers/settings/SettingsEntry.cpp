//
// Created by yunp on 2018/4/6.
//

#include "SettingsEntry.h"
#include <iostream>
#include "../../commons/LocalStorageManager.h"
#include "../../commons/node_globals.h"
#include "../../commons/SRWindows.h"
#include <boost/format.hpp>
#include <emscripten/html5.h>
#include "../../commons/ElectronGlobals.h"

using namespace emscripten;

namespace sr {
    SettingsEntry *SettingsEntry::__instance = nullptr;

    SettingsEntry *SettingsEntry::getInstance() {
        if (!SettingsEntry::__instance) {
            SettingsEntry::__instance = new SettingsEntry();
        }
        return SettingsEntry::__instance;
    }

    SettingsEntry::SettingsEntry() :
            _inputDistDirPath(DOCUMENT_QUERY_SELECTOR("#distDirPath")),
            _audioBpsInput(DOCUMENT_QUERY_SELECTOR("#audioBpsInput")),
            _videoBpsInput(DOCUMENT_QUERY_SELECTOR("#videoBpsInput")) {

        initUI();
        addListeners();
    }

    void SettingsEntry::initUI() {
        _audioBpsInput.set("value", LocalStorageManager::getAudioBps());
        _videoBpsInput.set("value", LocalStorageManager::getVideoBps());
        _inputDistDirPath.set("value", LocalStorageManager::getSRLibraryDirectory());
    }

    EMSCRIPTEN_BINDINGS(Settings_Entry) {
        function("SettingsEntry_audioBpsInput_changeHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<SettingsEntry *>(__this.as<unsigned long>());
            LocalStorageManager::setAudioBps(self->get_audioBpsInput()["value"]);
        });

        function("SettingsEntry_videoBpsInput_changeHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<SettingsEntry *>(__this.as<unsigned long>());
            LocalStorageManager::setVideoBps(self->get_videoBpsInput()["value"]);
        });
    }

    static EM_BOOL btnBrowserForDistDir_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent,
                                                       void *userData) {
        auto self = static_cast<SettingsEntry *>(userData);
        val result = ELECTRON["remote"]["dialog"]
                .call<val>("showOpenDialog",
                           ELECTRON["remote"].call<val>("getCurrentWindow"),
                           MAKE_JSON_OBJECT(R"({"title":"\u9009\u62e9\u76ee\u5f55","properties":["openDirectory"]})")
                );
        if (!result.isUndefined()) {
            val path = result[0];
            val p = self->get_inputDistDirPath();
            p.set("value", path);
            LocalStorageManager::setSRLibraryDirectory(path);
        }
        return 0;
    }

    static EM_BOOL btnOpenVideoLibrary_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent,
                                                      void *userData) {

        SRWindows::showVideoLibraryWindow();
        return 0;
    }


    void SettingsEntry::addListeners() {
        _audioBpsInput.set("onchange", val::global("Function").new_(
                (boost::format("Module.SettingsEntry_audioBpsInput_changeHandler(%1%);") %
                 (unsigned long) this).str()));
        _videoBpsInput.set("onchange", val::global("Function").new_(
                (boost::format("Module.SettingsEntry_videoBpsInput_changeHandler(%1%);") %
                 (unsigned long) this).str()));
        emscripten_set_click_callback("btnBrowserForDistDir", this, 0, &btnBrowserForDistDir_clickedHandler);
        emscripten_set_click_callback("btn-open-video-library", this, 0, &btnOpenVideoLibrary_clickedHandler);
    }

    const val &SettingsEntry::get_audioBpsInput() const {
        return _audioBpsInput;
    }

    const val &SettingsEntry::get_videoBpsInput() const {
        return _videoBpsInput;
    }

    const val &SettingsEntry::get_inputDistDirPath() const {
        return _inputDistDirPath;
    }
}
