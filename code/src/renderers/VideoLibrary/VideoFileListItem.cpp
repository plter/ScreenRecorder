//
// Created by yunp on 2018/4/10.
//

#include "VideoFileListItem.h"
#include "../../commons/node_globals.h"
#include <boost/format.hpp>
#include "../../commons/LocalStorageManager.h"
#include <fstream>
#include <emscripten.h>
#include "../../commons/SRWindows.h"
#include "../../commons/SRGlobals.h"
#include "../../commons/ElectronGlobals.h"

using namespace emscripten;

namespace sr {
    VideoFileListItem::VideoFileListItem(const val &fileName) :
            _htmlNode(DOCUMENT.call<val>("createElement", val("div"))),
            _progressBar(val::undefined()),
            progressMessageDiv(val::undefined()),
            _jq(JQUERY(_htmlNode)),
            _fileName(fileName),
            savedPath(val::undefined()) {

        _jq.call<void>("addClass", val("video-list-item"));
        _jq.call<void>("html", val(R"(
<div class="export-progress"></div>
<div class="video-list-item-file-name"></div>
<div style="width:5px;"></div>
<div class="export-progress-message"></div>
<div style="width:5px;"></div>
<div class="video-list-item-operations btn-group" role="group">
  <button class="btn btn-default btn-sm btn-play-srv"><span style="color:green;" class="glyphicon glyphicon-play"></span></button>
  <button class="btn btn-default btn-sm btn-export"><span class="glyphicon glyphicon-export"></span></button>
  <button class="btn btn-default btn-sm btn-show-in-folder"><span class="glyphicon glyphicon-eye-open"></span></button>
</div>
)"));
        _jq.call<val>("find", val(".video-list-item-file-name")).call<void>("html", fileName);
        _progressBar = _jq.call<val>("find", val(".export-progress"));
        progressMessageDiv = _jq.call<val>("find", val(".export-progress-message"));
        addListeners();
    }

    const val &VideoFileListItem::get_htmlNode() const {
        return _htmlNode;
    }

    EMSCRIPTEN_BINDINGS(VideoLibrary_videoFileListItem) {
        function("VideoLibrary_videoFileListItem_btnShowInFolderClicked",
                 LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
                     reinterpret_cast<VideoFileListItem *>(__this)->onBtnShowInFolderClickedHandler();
                 });
        function("VideoLibrary_videoFileListItem_btnPlaySrvClicked", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
            reinterpret_cast<VideoFileListItem *>(__this)->onBtnPlaySrvClickedHandler();
        });
        function("VideoLibrary_videoFileListItem_btnExportClicked", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
            reinterpret_cast<VideoFileListItem *>(__this)->onBtnExportClickedHandler();
        });
    }

    void VideoFileListItem::addListeners() {
        _jq.call<val>("find", val(".btn-show-in-folder")).call<void>("click", JS_FUNCTION((boost::format(R"(
Module.VideoLibrary_videoFileListItem_btnShowInFolderClicked(%1%);
)") % (unsigned long) this).str()));
        _jq.call<val>("find", val(".btn-play-srv")).call<void>("click", JS_FUNCTION((boost::format(R"(
Module.VideoLibrary_videoFileListItem_btnPlaySrvClicked(%1%);
)") % (unsigned long) this).str()));
        _jq.call<val>("find", val(".btn-export")).call<void>("click", JS_FUNCTION((boost::format(R"(
Module.VideoLibrary_videoFileListItem_btnExportClicked(%1%);
)") % (unsigned long) this).str()));
    }

    void VideoFileListItem::onBtnShowInFolderClickedHandler() {
        if (!savedPath.isUndefined() && !savedPath.isNull()) {
            ELECTRON["shell"].call<void>("showItemInFolder", savedPath);
        } else {
            val path = NODE_PATH_JOIN(LocalStorageManager::getSRLibraryDirectory(), _fileName);
            ELECTRON["shell"].call<void>("showItemInFolder", path);
        }
    }

    void VideoFileListItem::onBtnPlaySrvClickedHandler() {
        showPlayerWindow();
    }

    void VideoFileListItem::showPlayerWindow() {
        LocalStorageManager::setSrvFileToBeOpen(_fileName);
        SRWindows::showSRVPlayerWindow();
    }

    void VideoFileListItem::onBtnExportClickedHandler() {
        if (!_exportVideoTask) {
            val options = MAKE_JSON_OBJECT(R"({"filters":[{"name":"Video","extensions":["mp4"]}]})");
            val defaultPath = val("Unnamed");
            val lastPointIndex = _fileName.call<val>("lastIndexOf", val("."));
            if (lastPointIndex.as<int>() > -1) {
                defaultPath = _fileName.call<val>("substring", val(0), lastPointIndex);
            }
            options.set("defaultPath", defaultPath);
            savedPath = ELECTRON["remote"]["dialog"].call<val>("showSaveDialog",
                                                               ELECTRON["remote"].call<val>("getCurrentWindow"),
                                                               options);
            if (!savedPath.isNull() && !savedPath.isUndefined()) {
                _exportVideoTask = std::make_shared<ExportVideoTask>(savedPath, _fileName);
                _exportVideoTask->setOnProgress([&](unsigned long time, unsigned long total, std::wstring message) {
                    auto percent = time * 100 / total;
                    if (percent > 100) {
                        percent = 100;
                    }
                    _progressBar.call<void>("css", val("width"), (boost::format("%1%") % percent).str() + "%");
                    progressMessageDiv.call<void>("html", message);
                });
                _exportVideoTask->start();
            }
        } else {
            if (!_exportVideoTask->is_taskRunning()) {
                _exportVideoTask->start();
            } else {
                EM_ASM(alert("导出进程正在运行，无需重复执行该操作"));
            }
        }
    }
}