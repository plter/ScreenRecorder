//
// Created by yunp on 2018/4/10.
//

#include "SRWindows.h"
#include "node_globals.h"
#include <emscripten/bind.h>
#include <emscripten.h>
#include "SRGlobals.h"
#include "ElectronGlobals.h"
#include "LocalStorageManager.h"

using namespace emscripten;

namespace sr {
    void SRWindows::showVideoLibraryWindow() {
        val path = NODE_PATH_JOIN(__DIR_NAME, val("VideoLibrary.html"));

        val winOptions = val::object();
        winOptions.set("width", val(600));
        winOptions.set("height", val(600));
        winOptions.set("title", val("VideoLibrary"));

        val webPreferences = val::object();
        webPreferences.set("nodeIntegrationInWorker", val(true));
        winOptions.set("webPreferences", webPreferences);

        val win = showWindow(path, winOptions);
//        win["webContents"].call<void>("openDevTools");
    }

    void SRWindows::showSRVPlayerWindow() {
        val path = NODE_PATH_JOIN(__DIR_NAME, val("SRVPlayer.html"));

        val winOptions = val::object();
        winOptions.set("width", val(1024));
        winOptions.set("minWidth", val(1024));
        winOptions.set("height", val(660));
        winOptions.set("minHeight", val(660));
        winOptions.set("title", val("SRVPlayer"));

        val win = showWindow(path, winOptions);
//    win["webContents"].call<void>("openDevTools");
    }

    emscripten::val SRWindows::showWindow(emscripten::val &path, emscripten::val &windowOptions) {
        val urlOptions = val::object();
        urlOptions.set("protocol", val("file"));
        urlOptions.set("slashes", val(true));
        urlOptions.set("pathname", path);
        val url = NODE_URL.call<val>("format", urlOptions);

        val BrowserWindow = ELECTRON["remote"]["BrowserWindow"];
        val win = SRGlobals::JavaScriptObjectNew1(BrowserWindow, windowOptions);
        win.call<void>("loadURL", url);
        return win;
    }

    void SRWindows::showAboutWindow() {
        val options = val::object();
        options.set("width", 500);
        options.set("height", 360);
        options.set("modal", true);
        options.set("parent", ELECTRON["remote"].call<val>("getCurrentWindow"));

        val path = NODE_PATH_JOIN(__DIR_NAME, val("about.html"))
        showWindow(path, options);
    }

    void SRWindows::showSettingsWindow() {
        val options = val::object();
        options.set("width", 500);
        options.set("height", 260);
        options.set("modal", true);
        options.set("parent", ELECTRON["remote"].call<val>("getCurrentWindow"));

        val path = NODE_PATH_JOIN(__DIR_NAME, val("settings.html"))
        showWindow(path, options);
    }

    emscripten::val SRWindows::showChooseScreenStreamWindow() {
        LocalStorageManager::removeSelectScreenStreamID();

        val options = val::object();
        options.set("width", 500);
        options.set("height", 600);
        options.set("modal", true);
        options.set("parent", ELECTRON["remote"].call<val>("getCurrentWindow"));

        val path = NODE_PATH_JOIN(__DIR_NAME, val("ScreenChooser.html"))
        return showWindow(path, options);
    }

    emscripten::val SRWindows::showPendingToStartWindows() {
        val options = val::object();
        options.set("width", val(320));
        options.set("height", val(240));
        options.set("center", val(true));
        options.set("alwaysOnTop", val(true));
//        options.set("frame", val(false));
//        options.set("transparent", val(true));

        val path = NODE_PATH_JOIN(__DIR_NAME, val("PendingToStart.html"))
        return showWindow(path, options);
    }
}
