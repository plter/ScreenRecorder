//
// Created by yunp on 2018/4/9.
//

#include "VideoLibraryEntry.h"
#include "../../commons/LocalStorageManager.h"
#include "../../commons/node_globals.h"
#include "../../commons/SRGlobals.h"
#include <boost/format.hpp>

using namespace emscripten;

namespace sr {
    VideoLibraryEntry *VideoLibraryEntry::__instance = nullptr;

    VideoLibraryEntry *VideoLibraryEntry::getInstance() {
        if (__instance == nullptr) {
            __instance = new VideoLibraryEntry();
        }
        return __instance;
    }

    VideoLibraryEntry::VideoLibraryEntry() : _panelBody(DOCUMENT_QUERY_SELECTOR(".panel-body")) {
        buildUI();
    }

    void VideoLibraryEntry::buildUI() {
        val libDir = LocalStorageManager::getSRLibraryDirectory();
        val files = NODE_FS.call<val>("readdirSync", libDir);
        files.call<void>("sort");
        files.call<void>("reverse");

        auto len = files["length"].as<int>();
        for (int i = 0; i < len; ++i) {
            val f = files[i];
            if (f.call<val>("endsWith", val(SRV_FILE_TYPE)).as<bool>()) {
                auto item = std::make_shared<VideoFileListItem>(f);
                _items.push_back(item);
                _panelBody.call<void>("appendChild", item->get_htmlNode());
            }
        }
    }
}