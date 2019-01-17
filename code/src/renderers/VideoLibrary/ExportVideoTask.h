//
// Created by yunp on 2018/4/18.
//

#pragma once

#include <emscripten/bind.h>
#include <functional>
#include "../../commons/SRVReaderTask.h"
#include <memory>

namespace sr {
    class ExportVideoTask {
    private:
        emscripten::val _targetPath;
        emscripten::val _srcFileName;
        emscripten::val _ffmpegPath;
        emscripten::val _currentInFile;
        emscripten::val middleMp4File;
        std::shared_ptr<sr::SRVReaderTask> srvReaderTask;
        bool _taskRunning;
        unsigned long _currentWebMDuration;
        std::function<void(unsigned long, unsigned long, std::wstring message)> _onProgress;

        /**
         * 说明，由HTML5功能录制的视频文件直接用ffmpeg导出会出现视频与音频不同步的问题，为了解决这个问题，
         * 必须将视频流以拷贝的方式进行转码，即声明参数 -vcodec copy ，这样以来，无法为视频打水印，如果软件
         * 还未注册，则需要打水印，所以视频导出分成两个阶段，第一个阶段是将webm中的视频通道拷贝到mp4文件中，
         * 第二个阶段根据软件是否注册来判断该打码还是拷贝
         */
        static const int EXPORT_PHASE_COPY_WEBM_TO_MP4 = 1;
        static const int EXPORT_PHASE_CHECK_IF_ADD_WATER_MARKER_OR_COPY_TO_OUT = 2;
        int exportPhase;

    public:
        explicit ExportVideoTask(const emscripten::val &_targetPath, const emscripten::val &srcFileName);

        void start();

        emscripten::val getFfmpegPath();


        bool is_taskRunning() const;

        void onExportProcessOutputTimeHandler(unsigned long time);

        const std::function<void(unsigned long, unsigned long, std::wstring message)> &getOnProgress() const;

        void setOnProgress(const std::function<void(unsigned long, unsigned long, std::wstring message)> &onProgress);

        void publishProgress(unsigned long current, unsigned long total, std::wstring message);

        void onExportProcessCompletedHandler(int code);

    private:
        void startFfmpegConverter(emscripten::val inFile, emscripten::val outFile);
    };
}
