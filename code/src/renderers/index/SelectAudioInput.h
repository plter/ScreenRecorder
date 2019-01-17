//
// Created by yunp on 2018/4/6.
//

#ifndef SCREENRECORDER_SELECTAUDIOINPUT_H
#define SCREENRECORDER_SELECTAUDIOINPUT_H

#include <emscripten/val.h>
#include <map>

namespace sr {
    class SelectAudioInput {
    private:
        emscripten::val _selectAudioInput;
        std::map<std::string, emscripten::val> _deviceInfoMap;
    public:
        emscripten::val &get_selectAudioInput();

    public:
        SelectAudioInput();

        void loadAvailableDevices();

        void onLoadedAvailableDevices(const emscripten::val &allDeviceInformation);

        void addListeners();

        void selectAudioInputChangedHandler();

        emscripten::val getSelectedDeviceInfo();
    };
}

#endif //SCREENRECORDER_SELECTAUDIOINPUT_H
