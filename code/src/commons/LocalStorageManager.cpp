//
// Created by yunp on 2018/4/5.
//

#include "LocalStorageManager.h"
#include "node_globals.h"

using namespace emscripten;
namespace sr {
    val LocalStorageManager::getSRLibraryDirectory() {
        val r = LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("destDir"));
        if (r.isUndefined() || r.isNull()) {
            r = DEFAULT_SCREEN_RECORDER_LIBRARY;
        }
        return r;
    }

    void LocalStorageManager::setSRLibraryDirectory(emscripten::val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("destDir"), value);
    }

    emscripten::val LocalStorageManager::getAudioBps() {
        val r = LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("audioBps"));
        if (r.isNull() || r.isUndefined()) {
            r = val(DEFAULT_AUDIO_BPS);
        }
        return r;
    }

    void LocalStorageManager::setAudioBps(val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("audioBps"), value);
    }

    emscripten::val LocalStorageManager::getVideoBps() {
        val r = LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("videoBps"));
        if (r.isNull() || r.isUndefined()) {
            r = val(DEFAULT_VIDEO_BPS);
        }
        return r;
    }

    void LocalStorageManager::setVideoBps(val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("videoBps"), value);
    }

    void LocalStorageManager::setAudioInputDeviceId(emscripten::val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("audioInputDeviceId"), value);
    }

    emscripten::val LocalStorageManager::getAudioInputDeviceId() {
        return LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("audioInputDeviceId"));
    }

    void LocalStorageManager::setSelectedScreenStreamID(emscripten::val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("selectedScreenStreamID"), value);
    }

    emscripten::val LocalStorageManager::getSelectedScreenStreamID() {
        return LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("selectedScreenStreamID"));
    }

    void LocalStorageManager::removeSelectScreenStreamID() {
        LOCAL_STORAGE.call<val>("removeItem", MAKE_STORAGE_KEY("selectedScreenStreamID"));
    }

    void LocalStorageManager::setSrvFileToBeOpen(const emscripten::val &value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("srvFileToBeOpen"), value);
    }

    emscripten::val LocalStorageManager::getSrvFileToBeOpen() {
        return LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("srvFileToBeOpen"));
    }

    void LocalStorageManager::setBps(emscripten::val value) {
        LOCAL_STORAGE.call<void>("setItem", MAKE_STORAGE_KEY("bps"), value);
    }

    emscripten::val LocalStorageManager::getBps() {
        val r = LOCAL_STORAGE.call<val>("getItem", MAKE_STORAGE_KEY("bps"));
        if (r.isNull() || r.isUndefined()) {
            r = val(DEFAULT_BPS);
        }
        return r;
    }
}