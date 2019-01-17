//
// Created by yunp on 2018/4/5.
//

#include "Entry.h"
#include <iostream>
#include <emscripten.h>
#include "../../commons/node_globals.h"
#include "../../commons/LocalStorageManager.h"
#include "../../commons/SRWindows.h"
#include <boost/format.hpp>
#include "../../commons/SRGlobals.h"
#include <emscripten/bind.h>
#include "../../commons/ElectronGlobals.h"


using namespace emscripten;

namespace sr {

#define ENTRY_SELF(jsThis) (reinterpret_cast<Entry*>(jsThis))

    Entry::Entry() :
            _btnStartOrStop(DOCUMENT_QUERY_SELECTOR("#btn-start-or-stop")),
            _btnPauseOrResume(DOCUMENT_QUERY_SELECTOR("#btn-pause-or-resume")),
            _currentStream(val::undefined()),
            _mediaRecorder(val::undefined()),
            mainMenu(val::undefined()),
            _recordState(NORMAL) {

        initUI();
        addListeners();
    }

    void Entry::initUI() {
        set_recordState(NORMAL);
        this->buildMainMenu();
    }

    EMSCRIPTEN_BINDINGS(Index_Entry) {
        function("Index_Entry_global_startOrStopHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<Entry *>(__this.as<unsigned long>());
            self->global_startOrStopHandler();
        });

        function("Index_Entry_global_pauseOrResumeHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<Entry *>(__this.as<unsigned long>());
            self->global_pauseOrResumeHandler();
        });

        function("IndexEntry_ScreenChooserWindowClosedHandler", (void (*)(val)) [](val __this) {
            auto self = reinterpret_cast<Entry *>(__this.as<unsigned long>());
            self->screenChooserWindowClosedHandler();
        });

        function("Index_Entry_onGotAudioStream", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, val audioStream) {
            ENTRY_SELF(__this)->onGotAudioStreamHandler(audioStream);
        });

        function("Index_Entry_onPendingToStartWindowClosed", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this) {
            ENTRY_SELF(__this)->onPendingToStartWindowClosedHandler();
        });

        function("Index_Entry_onGotVideoStream",
                 (void (*)(unsigned long, val)) [](unsigned long __this, val videoStream) {
                     ENTRY_SELF(__this)->onGetVideoStreamHandler(videoStream);
                 });

        function("Index_Entry_onRecordDataAvailable", LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, val data) {
            ENTRY_SELF(__this)->onRecordDataAvailableHandler(data);
        });

        function("IndexEntry_aboutMenuItemClickedHandler", LAMBDA_TO_FUNCTION_POINTER() {
            SRWindows::showAboutWindow();
        });

        function("IndexEntry_srLibraryMenuItemClickedHandler", LAMBDA_TO_FUNCTION_POINTER() {
            SRWindows::showVideoLibraryWindow();
        });

        function("IndexEntry_settingsMenuItemClickedHandler", LAMBDA_TO_FUNCTION_POINTER() {
            SRWindows::showSettingsWindow();
        });

    }

    static int btnStartOrStop_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
        auto self = static_cast<Entry *>(userData);
        self->btnStartOrStopClickedHandler();
        return 0;
    }

    static int btnPauseOrResume_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
        auto self = static_cast<Entry *>(userData);
        self->btnPauseOrResumeClickedHandler();
        return 0;
    }

    void Entry::addListeners() {
        emscripten_set_click_callback("btn-show-main-menu", this, 0, &btnShowMainMenu_clickedHandler);
        emscripten_set_click_callback("btn-start-or-stop", this, 0, &btnStartOrStop_clickedHandler);
        emscripten_set_click_callback("btn-pause-or-resume", this, 0, &btnPauseOrResume_clickedHandler);

        //add ipc listeners
        ELECTRON["ipcRenderer"].call<val>("on", val("startOrStop"), JS_FUNCTION(
                val("event"),
                (boost::format(R"(Module.Index_Entry_global_startOrStopHandler(%1%);event.returnValue=1;)") %
                 (unsigned long) this).str()));
        ELECTRON["ipcRenderer"].call<val>("on", val("pauseOrResume"), JS_FUNCTION(
                val("event"),
                (boost::format(R"(Module.Index_Entry_global_pauseOrResumeHandler(%1%);event.returnValue=1;)") %
                 (unsigned long) this).str()));
    }

    EM_BOOL
    Entry::btnShowMainMenu_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {

        val options = val::object();
        options.set("x", 8);
        options.set("y", 60);
        ((Entry *) userData)->mainMenu.call<void>("popup", options);
        return 0;
    }

    void Entry::global_startOrStopHandler() {
        this->btnStartOrStopClickedHandler();
    }

    void Entry::global_pauseOrResumeHandler() {
        this->btnPauseOrResumeClickedHandler();
    }

    void Entry::btnStartOrStopClickedHandler() {
        switch (_recordState) {
            case NORMAL:
            case STOPPED:
                startRecord();
                break;
            case RECORDING:
                stopRecord();
                break;
            default:
                assert("Unknown record state");
        }
    }

    void Entry::btnPauseOrResumeClickedHandler() {
        switch (_recordState) {
            case RECORDING:
                pauseRecord();
                break;
            case PAUSED:
                resumeRecord();
                break;
            default:
                assert("Unknown record state");
        }
    }

    void Entry::disableAllButtons() {
        disableStopOrStartButton();
        disablePauseOrResumeButton();
    }

    void Entry::disableStopOrStartButton() {
        _btnStartOrStop.set("disabled", val(true));
    }

    void Entry::disablePauseOrResumeButton() {
        _btnPauseOrResume.set("disabled", val(true));
    }

    void Entry::enableStartOrStopButton() {
        _btnStartOrStop.set("disabled", val(false));
    }

    void Entry::enablePauseOrResumeButton() {
        _btnPauseOrResume.set("disabled", val(false));
    }

    RecordStatus Entry::get_recordState() const {
        return _recordState;
    }

    void Entry::set_recordState(RecordStatus state) {
        this->_recordState = state;
        switch (state) {
            case NORMAL:
                disablePauseOrResumeButton();
                enableStartOrStopButton();
                EM_ASM($("#btn-start-or-stop").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-record'></span>"));
                EM_ASM($("#btn-pause-or-resume").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-pause'></span>"));
                break;
            case RECORDING:
                enablePauseOrResumeButton();
                enableStartOrStopButton();
                EM_ASM($("#btn-start-or-stop").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-stop'></span>"));
                EM_ASM($("#btn-pause-or-resume").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-pause'></span>"));
                dispatchRecordingEvent();
                break;
            case STOPPED:
                enableStartOrStopButton();
                disablePauseOrResumeButton();
                EM_ASM($("#btn-start-or-stop").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-record'></span>"));
                EM_ASM($("#btn-pause-or-resume").html(
                        "<span style='color:#dd6a5a;' class='glyphicon glyphicon-pause'></span>"));
                dispatchStoppedEvent();
                ELECTRON["remote"].call<val>("getCurrentWindow").call<void>("restore");
                break;
            case PAUSED:
                disableStopOrStartButton();
                enablePauseOrResumeButton();
                EM_ASM($("#btn-pause-or-resume").html(
                        "<span style='color:green;' class='glyphicon glyphicon-repeat'></span>"));
                dispatchPausedEvent();
                break;
            case CHECKING_SCREEN_STREAM:
                disableAllButtons();
                break;
            case CHECKING_AUDIO_STREAM:
                disableAllButtons();
                break;
            case PENDING_TO_START:
                disableAllButtons();
                break;
        }
    }

    void Entry::startRecord() {
        /* 开始录制视频的流程
         * 1. 检查视频输入
         * 2. 检查音频输入
         * 3. 开始录制*/

        chooseScreenStream();
    }

    void Entry::startCheckVideoStream() {
        set_recordState(CHECKING_SCREEN_STREAM);
    }


    void Entry::chooseScreenStream() {
        set_recordState(CHECKING_SCREEN_STREAM);
        val win = SRWindows::showChooseScreenStreamWindow();
        win.call<void>("once",
                       val("closed"),
                       SRGlobals::JSFunction(
                               (boost::format(R"(Module.IndexEntry_ScreenChooserWindowClosedHandler(%1%))") %
                                (unsigned long) this).str()
                       )
        );
    }


    void Entry::screenChooserWindowClosedHandler() {
        val sid = LocalStorageManager::getSelectedScreenStreamID();
        if (!sid.isNull() && !sid.isUndefined()) {
            SRGlobals::JSFunction2("screenStreamID", "cppSelf", R"(
(async function(){
  try{
    var width = screen.width*2;
    var height = screen.height*2;
    var videoStream = await navigator.mediaDevices.getUserMedia({
      audio: false,
      video: {
        mandatory: {
          chromeMediaSource: "desktop",
          chromeMediaSourceId: screenStreamID,
          minWidth: width,
          maxWidth: width,
          minHeight: height,
          maxHeight: height
        }
      }
    });

    Module.Index_Entry_onGotVideoStream(cppSelf,videoStream);
  }catch(e){
    alert("Can not get video stream");
  }
})()
)")(sid, (unsigned long) this);
        } else {
            set_recordState(NORMAL);
        }
    }

    void Entry::onGetVideoStreamHandler(const emscripten::val &videoStream) {
        _currentStream = videoStream;
        getAudioStream();
    }

    void Entry::getAudioStream() {
        set_recordState(CHECKING_AUDIO_STREAM);
        val options = val::object();
        options.set("audio", _selectAudioInput.getSelectedDeviceInfo());

        val::global("navigator")["mediaDevices"].call<val>("getUserMedia", options)
                .call<val>("then",
                           JS_FUNCTION(
                                   val("resultStream"),
                                   val((boost::format(
                                           R"(
setTimeout(()=>{
  Module.Index_Entry_onGotAudioStream(%1%,resultStream);
},100);
)") % (unsigned long) this).str()
                                   )
                           )
                )
                .call<val>("catch", JS_FUNCTION(val("errorEvent"),
                                                val(R"(alert("Can not get audio stream");console.log(errorEvent);)")));
    }

    void Entry::onGotAudioStreamHandler(const emscripten::val &audioStream) {
        _currentStream.call<val>("addTrack", audioStream.call<val>("getTracks")[0]);

        ELECTRON["remote"].call<val>("getCurrentWindow").call<void>("minimize");

        val options = val::object();
        options.set("mimeType", val("video/webm;codecs=h264"));
        options.set("audioBitsPerSecond", LocalStorageManager::getAudioBps());
        options.set("videoBitsPerSecond", LocalStorageManager::getVideoBps());
        _streamQueue = std::make_shared<StreamQueue>();
        _mediaRecorder = val::global("MediaRecorder").new_(_currentStream, options);
        _mediaRecorder.set("ondataavailable",
                           JS_FUNCTION(val("event"),
                                       (boost::format("Module.Index_Entry_onRecordDataAvailable(%1%,event.data)") %
                                        (unsigned long) this).str()));

        set_recordState(PENDING_TO_START);
        showPendingToStartWindows();
    }

    void Entry::stopRecord() {
        _mediaRecorder.call<void>("stop");
        set_recordState(STOPPED);
    }

    void Entry::pauseRecord() {
        _mediaRecorder.call<void>("pause");
        set_recordState(PAUSED);
    }

    void Entry::resumeRecord() {
        _mediaRecorder.call<void>("resume");
        set_recordState(RECORDING);
    }


    void Entry::showPendingToStartWindows() {
        val win = SRWindows::showPendingToStartWindows();

        win.call<val>("once", val("closed"), JS_FUNCTION(
                (boost::format("Module.Index_Entry_onPendingToStartWindowClosed(%1%)") % (unsigned long) this).str()));
    }

    void Entry::onPendingToStartWindowClosedHandler() {
        _mediaRecorder.call<void>("start", val(SRV_WEBM_BLOB_DURATION));
        set_recordState(RECORDING);
    }

    void Entry::dispatchRecordingEvent() {
        ELECTRON["ipcRenderer"].call<void>("send", std::string("recording"));
    }

    void Entry::dispatchStoppedEvent() {
        ELECTRON["ipcRenderer"].call<void>("send", std::string("stopped"));
    }

    void Entry::dispatchPausedEvent() {
        ELECTRON["ipcRenderer"].call<void>("send", std::string("paused"));
    }

    void Entry::onRecordDataAvailableHandler(emscripten::val data) {
        _streamQueue->appendData(std::move(data));
    }

    void Entry::buildMainMenu() {
        mainMenu = SRGlobals::JavaScriptObjectNew(ELECTRON["remote"]["Menu"]);

        mainMenu.call<void>("append", createMenuItem(
                val(std::wstring(L"视频库")),
                SRGlobals::JSFunctionWB(LR"(Module.IndexEntry_srLibraryMenuItemClickedHandler())"),
                val("ic_video_library_black_18dp.png"),
                val::undefined()
        ));

        mainMenu.call<void>("append", createMenuItem(
                val(std::wstring(L"设置")),
                SRGlobals::JSFunctionWB(LR"(Module.IndexEntry_settingsMenuItemClickedHandler())"),
                val("ic_settings_black_18dp.png"),
                val::undefined()
        ));

        mainMenu.call<void>("append", createMenuItem(
                val(std::wstring(L"关于 ScreenRecorder")),
                SRGlobals::JSFunctionWB(LR"(Module.IndexEntry_aboutMenuItemClickedHandler())"),
                val("ic_info_outline_black_18dp.png"),
                val::undefined()
        ));

        mainMenu.call<void>("append", createMenuItem(
                val::undefined(),
                val::undefined(),
                val::undefined(),
                val("separator")
        ));

        mainMenu.call<void>("append", createMenuItem(
                val(std::wstring(L"退出")),
                SRGlobals::JSFunctionWB(LR"(window.close();)"),
                val("ic_highlight_off_black_18dp.png"),
                val::undefined()
        ));
    }

    emscripten::val
    Entry::createMenuItem(const emscripten::val &label, const emscripten::val &clickCallback,
                          const emscripten::val &iconName,
                          const emscripten::val &type) {
        val appRoot = APP_ROOT;
        val options = val::object();
        if (!label.isNull() && !label.isUndefined()) {
            options.set("label", label);
        }
        if (!iconName.isUndefined() && !iconName.isNull()) {
            val iconPath = NODE_PATH_JOIN(appRoot, std::string("res"), std::string("icons"),
                                          std::string("MaterialDesignIcons"),
                                          iconName);
            options.set("icon", iconPath);
        }
        if (!clickCallback.isNull() && !clickCallback.isUndefined()) {
            options.set("click", clickCallback);
        }
        if (!type.isUndefined() && !type.isNull()) {
            options.set("type", type);
        }
        return SRGlobals::JavaScriptObjectNew1(ELECTRON["remote"]["MenuItem"], options);
    }
}
