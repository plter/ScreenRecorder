//
// Created by yunp on 2018/4/6.
//

#include "SelectAudioInput.h"
#include <emscripten.h>
#include "../../commons/node_globals.h"
#include <iostream>
#include <boost/format.hpp>
#include "../../commons/LocalStorageManager.h"

using namespace emscripten;
namespace sr {
    SelectAudioInput::SelectAudioInput() : _selectAudioInput(DOCUMENT_QUERY_SELECTOR("#select-audio-input")) {
        loadAvailableDevices();
        addListeners();
    }

    EMSCRIPTEN_BINDINGS(Index_SelectAudioInput_loadAvailableDevices) {
        function("Index_SelectAudioInput_loadAvailableDevices", (void (*)(val, val)) [](val __this, val infos) {
            auto self = reinterpret_cast<SelectAudioInput *>(__this.as<unsigned long>());
            self->onLoadedAvailableDevices(infos);
        });
    }

    void SelectAudioInput::loadAvailableDevices() {
        emscripten_run_script((boost::format(R"(
(async function(){
  let infos = await navigator.mediaDevices.enumerateDevices();
  Module.Index_SelectAudioInput_loadAvailableDevices(%1%,infos);
})())") % (unsigned long) this).str().c_str());
    }

    val &SelectAudioInput::get_selectAudioInput() {
        return _selectAudioInput;
    }

    void SelectAudioInput::onLoadedAvailableDevices(const val &allDeviceInformation) {
        std::string html;
        auto length = allDeviceInformation["length"].as<int>();
        for (int i = 0; i < length; i++) {
            val info = allDeviceInformation[i];
            if (info["kind"].equals(val("audioinput"))) {
                val deviceId = info["deviceId"];
                val option = DOCUMENT_CREATE_ELEMENT("option");
                option.call<void>("setAttribute", val("value"), deviceId);
                option.set("innerHTML", info["label"]);
                _selectAudioInput.call<void>("appendChild", option);
                _deviceInfoMap.insert(std::pair<std::string, val>(deviceId.as<std::string>(), info));
            }
        }

        auto storedIdVal = LocalStorageManager::getAudioInputDeviceId();
        if (!storedIdVal.isNull() && !storedIdVal.isUndefined()) {
            auto storedId = storedIdVal.as<std::string>();
            if (_deviceInfoMap.count(storedId)) {
                auto storedInfo = _deviceInfoMap.at(storedId);
                _selectAudioInput.set("value", storedInfo["deviceId"]);
            }
        }
    }

    EMSCRIPTEN_BINDINGS(Index_SelectAudioInput_onchangeHandler) {
        function("Index_SelectAudioInput_onchangeHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<SelectAudioInput *>(__this.as<unsigned long>());
            self->selectAudioInputChangedHandler();
        });
    }

    void SelectAudioInput::addListeners() {
        _selectAudioInput.set("onchange",
                              JS_FUNCTION((boost::format(R"(Module.Index_SelectAudioInput_onchangeHandler(%1%))") %
                                           (unsigned long) this).str()));
    }

    void SelectAudioInput::selectAudioInputChangedHandler() {
        LocalStorageManager::setAudioInputDeviceId(_selectAudioInput["value"]);
    }

    emscripten::val SelectAudioInput::getSelectedDeviceInfo() {
        return _deviceInfoMap.at(_selectAudioInput["value"].as<std::string>());
    }
}