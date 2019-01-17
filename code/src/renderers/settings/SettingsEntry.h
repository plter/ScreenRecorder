//
// Created by yunp on 2018/4/6.
//

#ifndef SCREENRECORDER_SETTINGSENTRY_H
#define SCREENRECORDER_SETTINGSENTRY_H

#include <emscripten/val.h>
#include <emscripten/html5.h>

namespace sr {
    class SettingsEntry {
    private:
        static SettingsEntry *__instance;

        emscripten::val _inputDistDirPath;
    public:
        const emscripten::val &get_inputDistDirPath() const;

    private:
        emscripten::val _audioBpsInput;
    public:
        const emscripten::val &get_audioBpsInput() const;

        const emscripten::val &get_videoBpsInput() const;

    private:
        emscripten::val _videoBpsInput;

    public:
        SettingsEntry();

    public:
        static SettingsEntry *getInstance();

        void initUI();

        void addListeners();
    };
}


#endif //SCREENRECORDER_SETTINGSENTRY_H
