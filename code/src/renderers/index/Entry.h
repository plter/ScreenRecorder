//
// Created by yunp on 2018/4/5.
//

#pragma once

#include <emscripten/html5.h>
#include <emscripten/val.h>
#include "SelectAudioInput.h"
#include "StreamQueue.h"
#include <memory>

namespace sr {
    enum RecordStatus {
        NORMAL,//初始状态
        RECORDING,//正在录制
        STOPPED,//已停止
        PAUSED,//暂停状态
        PENDING_TO_START,//等待开始
        CHECKING_SCREEN_STREAM,//检查视频输入
        CHECKING_AUDIO_STREAM//检查音频输入
    };

    class Entry {

    private:
        emscripten::val _btnStartOrStop;
        emscripten::val _btnPauseOrResume;
        emscripten::val _currentStream;
        emscripten::val _mediaRecorder;
        emscripten::val mainMenu;
        SelectAudioInput _selectAudioInput;
        RecordStatus _recordState;
        std::shared_ptr<StreamQueue> _streamQueue;

    public:
        RecordStatus get_recordState() const;

        void set_recordState(RecordStatus state);

    private:

        void addListeners();

        void initUI();

        static EM_BOOL btnShowMainMenu_clickedHandler(int eventType, const EmscriptenMouseEvent *mouseEvent,
                                                      void *userData);

    public:
        Entry();

        void global_startOrStopHandler();

        void global_pauseOrResumeHandler();

        void btnStartOrStopClickedHandler();

        void btnPauseOrResumeClickedHandler();

        void disableAllButtons();

        void disableStopOrStartButton();

        void disablePauseOrResumeButton();

        void enableStartOrStopButton();

        void enablePauseOrResumeButton();

        void startRecord();

        void startCheckVideoStream();

        void chooseScreenStream();

        void screenChooserWindowClosedHandler();

        void onGetVideoStreamHandler(const emscripten::val &videoStream);

        void getAudioStream();

        void onGotAudioStreamHandler(const emscripten::val &audioStream);

        void stopRecord();

        void pauseRecord();

        void resumeRecord();

        void showPendingToStartWindows();

        void onPendingToStartWindowClosedHandler();

        void dispatchRecordingEvent();

        void dispatchStoppedEvent();

        void dispatchPausedEvent();

        void onRecordDataAvailableHandler(emscripten::val data);

        void buildMainMenu();

        emscripten::val createMenuItem(const emscripten::val &label, const emscripten::val &clickCallback,
                                       const emscripten::val &iconName,
                                       const emscripten::val &type);
    };
}
