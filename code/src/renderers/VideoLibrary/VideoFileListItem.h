//
// Created by yunp on 2018/4/10.
//

#ifndef SCREENRECORDER_VIDEOFILELISTITEM_H
#define SCREENRECORDER_VIDEOFILELISTITEM_H

#include <emscripten/bind.h>
#include "ExportVideoTask.h"
#include <memory>

namespace sr {
    class VideoFileListItem {
    private:
        emscripten::val _htmlNode;
        emscripten::val _progressBar;
        emscripten::val progressMessageDiv;
        emscripten::val _jq;
        emscripten::val _fileName;
        std::shared_ptr<sr::ExportVideoTask> _exportVideoTask;
        emscripten::val savedPath;

    public:
        const emscripten::val &get_htmlNode() const;

    public:
        explicit VideoFileListItem(const emscripten::val &fileName);

        void addListeners();

        void onBtnShowInFolderClickedHandler();

        void onBtnPlaySrvClickedHandler();

        void showPlayerWindow();

        void onBtnExportClickedHandler();
    };
}

#endif //SCREENRECORDER_VIDEOFILELISTITEM_H
