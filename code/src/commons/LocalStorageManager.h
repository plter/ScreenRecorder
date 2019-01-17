//
// Created by yunp on 2018/4/5.
//

#ifndef SCREENRECORDER_LOCALSTORAGEMANAGER_H
#define SCREENRECORDER_LOCALSTORAGEMANAGER_H

#define STORAGE_KEY_NAMESPACE "top.yunp.ScreenRecorder."
#define DEFAULT_AUDIO_BPS 128000
#define DEFAULT_VIDEO_BPS 6000000
#define DEFAULT_BPS 8000000
#define DEFAULT_SCREEN_RECORDER_LIBRARY NODE_PATH_JOIN(NODE_OS.call<val>("homedir"), val("SRLibrary"))

#define MAKE_STORAGE_KEY(name) emscripten::val(STORAGE_KEY_NAMESPACE name)

#include <emscripten/val.h>

#define LOCAL_STORAGE emscripten::val::global("localStorage")
namespace sr {
    class LocalStorageManager {
    public:
        static emscripten::val getSRLibraryDirectory();

        static void setSRLibraryDirectory(emscripten::val value);

        static emscripten::val getAudioBps();

        static void setAudioBps(emscripten::val value);

        static emscripten::val getVideoBps();

        static void setBps(emscripten::val value);

        static emscripten::val getBps();

        static void setVideoBps(emscripten::val value);

        static void setAudioInputDeviceId(emscripten::val value);

        static emscripten::val getAudioInputDeviceId();

        static void setSelectedScreenStreamID(emscripten::val value);

        static emscripten::val getSelectedScreenStreamID();

        static void removeSelectScreenStreamID();

        static emscripten::val getSrvFileToBeOpen();

        static void setSrvFileToBeOpen(const emscripten::val &value);
    };
}

#endif //SCREENRECORDER_LOCALSTORAGEMANAGER_H
