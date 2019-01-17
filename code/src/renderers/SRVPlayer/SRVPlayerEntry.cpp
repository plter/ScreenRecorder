//
// Created by yunp on 2018/4/18.
//

#include "SRVPlayerEntry.h"
#include "../../commons/node_globals.h"
#include "../../commons/LocalStorageManager.h"
#include "../../commons/worker/SRVReader.h"
#include <emscripten.h>
#include <boost/format.hpp>
#include <emscripten/html5.h>

sr::SRVPlayerEntry *sr::SRVPlayerEntry::__instance = nullptr;

using namespace emscripten;

sr::SRVPlayerEntry *sr::SRVPlayerEntry::getInstance() {
    if (__instance == nullptr) {
        __instance = new sr::SRVPlayerEntry();
    }
    return __instance;
}

sr::SRVPlayerEntry::SRVPlayerEntry() :
        _videoPlayer(DOCUMENT_QUERY_SELECTOR("#player")),
        _seekBar(DOCUMENT_QUERY_SELECTOR("#seek-bar")),
        _seekBarJq(JQUERY(_seekBar)),
        statusDiv(JQUERY(val("#status"))) {
    playVideo();
    addListeners();
}

void sr::SRVPlayerEntry::playVideo() {
    val videoName = sr::LocalStorageManager::getSrvFileToBeOpen();

    std::string path = "/SRLibrary/";
    path += videoName.as<std::string>();
    sr::SRVReader srv(path);
    try {
        srv.read();

        if (srv.hasSRVWebM()) {
            auto firstWebM = srv.getFirstSRVWebM();
            std::stringbuf sBuf;
            firstWebM->concatParts(sBuf);

            auto bufSize = sBuf.in_avail();
            auto cBuf = new char[bufSize];//这块内存不应该被释放，因为在JS中将长期使用
            sBuf.sgetn(cBuf, bufSize);

            val arrayBuffer = val(typed_memory_view(bufSize, cBuf));
            val blobParts = val::array();
            blobParts.call<void>("push", arrayBuffer);
            val blob = val::global("Blob").new_(blobParts);
            val url = val::global("URL").call<val>("createObjectURL", blob);
            _videoPlayer.set("src", url);

            _seekBarJq.call<void>("slider", val("option"), val("max"), val(firstWebM->getSrvDuration()));
            statusDiv.call<void>("remove");
        } else {
            EM_ASM(alert("无法读取视频文件"));
        }
    } catch (sr::SRVUnknownTypeException &e) {
        EM_ASM(
                alert("SRV文件格式不正确");
                window.close();
        );
    } catch (sr::SRVFileNotFoundException &e) {
        EM_ASM(
                alert("SRV文件不存在");
                window.close();
        );
    }
}

EMSCRIPTEN_BINDINGS(SRVPlayerEntry) {
    function("SRVPlayer_SRVPlayerEntry_playerTimeUpdate", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
        reinterpret_cast<sr::SRVPlayerEntry *>(__this)->playerTimeUpdateHandler();
    });
    function("SRVPlayer_SRVPlayerEntry_onSlide", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, unsigned int value) {
        reinterpret_cast<sr::SRVPlayerEntry *>(__this)->onSlideHandler(value);
    });
}

EM_BOOL btnPauseClickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    auto self = static_cast<sr::SRVPlayerEntry *>(userData);
    self->pauseCurrentVideo();
    return 0;
}

EM_BOOL btnPlayClickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    auto self = static_cast<sr::SRVPlayerEntry *>(userData);
    self->playCurrentVideo();
    return 0;
}


void sr::SRVPlayerEntry::addListeners() {
    _videoPlayer.call<void>("addEventListener", val("timeupdate"), JS_FUNCTION((boost::format(R"(
Module.SRVPlayer_SRVPlayerEntry_playerTimeUpdate(%1%);
)") % (unsigned long) this).str()));
    _seekBarJq.call<void>("on", val("slide"), JS_FUNCTION(val("event"), val("ui"), (boost::format(R"(
Module.SRVPlayer_SRVPlayerEntry_onSlide(%1%,ui.value);
)") % (unsigned long) this).str()));
    emscripten_set_click_callback("btn-pause", this, 0, &btnPauseClickedHandler);
    emscripten_set_click_callback("btn-play", this, 0, &btnPlayClickedHandler);
}

void sr::SRVPlayerEntry::playerTimeUpdateHandler() {
    _seekBarJq.call<void>("slider", val("option"), val("value"),
                          val(static_cast<unsigned int>(_videoPlayer["currentTime"].as<double>() * 1000)));
}

void sr::SRVPlayerEntry::onSlideHandler(unsigned int i) {
    _videoPlayer.set("currentTime", val(static_cast<double>(i) / 1000));
}

void sr::SRVPlayerEntry::pauseCurrentVideo() {
    _videoPlayer.call<void>("pause");
}

void sr::SRVPlayerEntry::playCurrentVideo() {
    _videoPlayer.call<void>("play");
}
