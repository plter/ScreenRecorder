//
// Created by yunp on 2018/4/18.
//

#include "ExportVideoTask.h"
#include "../../commons/node_globals.h"
#include <emscripten.h>
#include "../../commons/worker/SRVReader.h"
#include "../../commons/SRGlobals.h"
#include "../../commons/LocalStorageManager.h"
#include <boost/format.hpp>
#include <utility>
#include <emscripten/bind.h>
#include "../../commons/Res.h"

using namespace emscripten;

sr::ExportVideoTask::ExportVideoTask(const emscripten::val &targetPath, const emscripten::val &srcFileName) :
        _targetPath(targetPath),
        _srcFileName(srcFileName),
        _ffmpegPath(val::undefined()),
        _currentInFile(val::undefined()),
        middleMp4File(val::undefined()),
        _taskRunning(false),
        _currentWebMDuration(0),
        exportPhase(EXPORT_PHASE_COPY_WEBM_TO_MP4) {
}

void sr::ExportVideoTask::start() {
    _taskRunning = true;

    std::string inFileName = _srcFileName.as<std::string>();
    time_t timeStamp;
    time(&timeStamp);
    std::string outFileName = (boost::format(".%1%.%2%.tmp") % inFileName % timeStamp).str();
    std::string middleMp4FileName = outFileName + ".mp4";

    std::string inFileNODEFSPath = NODEFS_SR_LIBRARY;
    inFileNODEFSPath += "/";
    inFileNODEFSPath += inFileName;

    std::string outFileNODEFSPath = NODEFS_SR_TMP;
    outFileNODEFSPath += "/";
    outFileNODEFSPath += outFileName;

    srvReaderTask = std::make_shared<SRVReaderTask>();
    srvReaderTask->setProgressHandler(
            [=](std::string state, std::wstring message, unsigned long current, unsigned long total,
                unsigned long srvDuration) {
                if (state == "reading" || state == "writing") {
                    publishProgress(current, total, message);
                } else if (state == "complete") {
                    this->_currentWebMDuration = srvDuration;
                    this->_currentInFile = NODE_PATH_JOIN(NODE_OS.call<val>("tmpdir"), outFileName);
                    this->middleMp4File = NODE_PATH_JOIN(NODE_OS.call<val>("tmpdir"), middleMp4FileName);
                    this->exportPhase = EXPORT_PHASE_COPY_WEBM_TO_MP4;//第一阶段，导出成MP4
                    this->startFfmpegConverter(this->_currentInFile, this->middleMp4File);
                    srvReaderTask->terminate();
                    publishProgress(0, 1, L"文件准备完成");
                }
            }
    );
    srvReaderTask->setReadyHandler([=]() {
        srvReaderTask->mountNODEFS();
        srvReaderTask->encodeSrvToWebm(inFileNODEFSPath, outFileNODEFSPath);
    });
    publishProgress(0, 1, L"开始准备文件");
}

emscripten::val sr::ExportVideoTask::getFfmpegPath() {

    if (_ffmpegPath.isUndefined() || _ffmpegPath.isNull()) {
        val osType = NODE_OS.call<val>("type");
        val arch = NODE_OS.call<val>("arch");

        if (osType.equals(val("Darwin"))) {
            if (arch.equals(val("x64"))) {
                _ffmpegPath = NODE_PATH_JOIN(APP_ROOT, val("res"), val("bin"), val("ffmpeg_osx64"));
            } else {
                EM_ASM(alert("该功能只支持64位OSX"));
            }
        } else if (osType.equals(val("Windows_NT"))) {
            if (arch.equals(val("x64"))) {
                _ffmpegPath = NODE_PATH_JOIN(APP_ROOT, val("res"), val("bin"), val("ffmpeg_win64.exe"));
            } else if (arch.equals(val("x32")) || arch.equals(val("ia32"))) {
                _ffmpegPath = NODE_PATH_JOIN(APP_ROOT, val("res"), val("bin"), val("ffmpeg_win32.exe"));
            } else {
                EM_ASM(alert("这哪来的Windows，既不是32位，又不是64位，你想难为我啊？"));
            }
        } else if (osType.equals(val("Linux"))) {
            if (arch.equals(val("x64"))) {
                _ffmpegPath = NODE_PATH_JOIN(APP_ROOT, val("res"), val("bin"), val("ffmpeg_linux64"));
            } else if (arch.equals(val("x32")) || arch.equals(val("ia32"))) {
                _ffmpegPath = NODE_PATH_JOIN(APP_ROOT, val("res"), val("bin"), val("ffmpeg_linux32"));
            } else {
                EM_ASM(alert("这哪来的Linux，既不是32位，又不是64位，你想难为我啊？"));
            }
        } else {
            EM_ASM(alert("该功能不支持您的系统环境"));
        }
    }

    return _ffmpegPath;
}

bool sr::ExportVideoTask::is_taskRunning() const {
    return _taskRunning;
}

EMSCRIPTEN_BINDINGS(VideoLibrary_ExportVideoTask) {
    function("VideoLibrary_ExportVideoTask_onExportProcessOutputTime",
             LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, val time) {
                 reinterpret_cast<sr::ExportVideoTask *>(__this)->onExportProcessOutputTimeHandler(
                         time.as<unsigned long>());
             });
    function("VideoLibrary_ExportVideoTask_onExportProcessCompleted",
             LAMBDA_TO_FUNCTION_POINTER(unsigned long __this, val time) {
                 reinterpret_cast<sr::ExportVideoTask *>(__this)->onExportProcessCompletedHandler(
                         time.as<int>());
             });
}

void sr::ExportVideoTask::startFfmpegConverter(emscripten::val inFile, emscripten::val outFile) {
    val args = val::array();
    if (this->exportPhase == EXPORT_PHASE_COPY_WEBM_TO_MP4) {
        //第一阶段，拷贝视频
        args.call<void>("push", val("-y"));

        args.call<void>("push", val("-i"));
        args.call<void>("push", inFile);

        args.call<void>("push", val("-max_muxing_queue_size"));
        args.call<void>("push", val("1024"));

        args.call<void>("push", val("-vcodec"));
        args.call<void>("push", val("copy"));

        args.call<void>("push", val("-b:a"));
        args.call<void>("push", LocalStorageManager::getAudioBps());

        args.call<void>("push", val("-r"));
        args.call<void>("push", val("30"));

        args.call<void>("push", outFile);
    } else if (this->exportPhase == EXPORT_PHASE_CHECK_IF_ADD_WATER_MARKER_OR_COPY_TO_OUT) {
        //第二阶段，导出
        args.call<void>("push", val("-y"));

        args.call<void>("push", val("-i"));
        args.call<void>("push", inFile);

        args.call<void>("push", val("-max_muxing_queue_size"));
        args.call<void>("push", val("1024"));

        args.call<void>("push", val("-b:a"));
        args.call<void>("push", LocalStorageManager::getAudioBps());
        args.call<void>("push", val("-b:v"));
        args.call<void>("push", LocalStorageManager::getVideoBps());

        args.call<void>("push", val("-r"));
        args.call<void>("push", val("30"));

        // water marker >>>>>
        val drawTextArgs = val::array();
        drawTextArgs.call<void>("push", val("drawtext=fontfile="));
        drawTextArgs.call<void>("push", Res::getInstance()->getDefaultFontFileName());
        drawTextArgs.call<void>("push", val(":"));
        drawTextArgs.call<void>("push", val("fontsize=38:x=20:y=20:fontcolor=0x00C9DD:text=YUNP.TOP"));
        val drawTextArgsString = drawTextArgs.call<val>("join", val(""));
        args.call<void>("push", val("-vf"));
        args.call<void>("push", drawTextArgsString);
        // <<<<<<<<<<<<<<<<<<

        args.call<void>("push", outFile);
    } else {
        CONSOLE_LOG(val("No such a phase."));
    }

    val options = val::object();
    options.set("maxBuffer", val(20 * 1024 * 1024));
    options.set("cwd", Res::getInstance()->getFontsRoot());

//    CONSOLE_LOG(args.call<val>("join", val(" ")));

    val process = NodeGlobals::getInstance()->get_child_process().call<val>("execFile", getFfmpegPath(), args, options);
    process["stderr"].call<void>("on", val("data"), SRGlobals::JSFunction1("data", (boost::format(R"(
var result =  /time=(\d{2}):(\d{2}):(\d{2})\.(\d{2})/.exec(data);
if(result){
  var h = parseInt(result[1])*60*60*1000;
  var m = parseInt(result[2])*60*1000;
  var s = parseInt(result[3]*1000);
  var ms = parseInt(result[4])*10;
  Module.VideoLibrary_ExportVideoTask_onExportProcessOutputTime(%1%,h+m+s+ms);
}
)") % (unsigned long) this).str()));

    process.call<void>("on", val("close"), SRGlobals::JSFunction1("code", (boost::format(R"(
Module.VideoLibrary_ExportVideoTask_onExportProcessCompleted(%1%,code);
)") % (unsigned long) this).str()));
}

void sr::ExportVideoTask::onExportProcessOutputTimeHandler(unsigned long time) {
    if (exportPhase == EXPORT_PHASE_COPY_WEBM_TO_MP4) {
        publishProgress(time, _currentWebMDuration, L"正在生成");
    } else if (exportPhase == EXPORT_PHASE_CHECK_IF_ADD_WATER_MARKER_OR_COPY_TO_OUT) {
        publishProgress(time, _currentWebMDuration, L"正在导出");
    } else {
        publishProgress(time, _currentWebMDuration, L"");
    }
}

const std::function<void(unsigned long, unsigned long, std::wstring message)> &
sr::ExportVideoTask::getOnProgress() const {
    return _onProgress;
}

void sr::ExportVideoTask::setOnProgress(
        const std::function<void(unsigned long, unsigned long, std::wstring message)> &onProgress) {
    ExportVideoTask::_onProgress = onProgress;
}

void sr::ExportVideoTask::onExportProcessCompletedHandler(int code) {

    if (!_currentInFile.isUndefined() && !_currentInFile.isNull()) {
        if (NODE_FS.call<bool>("existsSync", _currentInFile)) {
            NODE_FS.call<void>("unlinkSync", _currentInFile);
        }
    }

    if (!code) {
        if (exportPhase == EXPORT_PHASE_COPY_WEBM_TO_MP4) {
            //进入第二阶段，打水印或者拷贝
            this->exportPhase = EXPORT_PHASE_CHECK_IF_ADD_WATER_MARKER_OR_COPY_TO_OUT;
            this->_currentInFile = this->middleMp4File;
            startFfmpegConverter(this->_currentInFile, this->_targetPath);
        } else if (exportPhase == EXPORT_PHASE_CHECK_IF_ADD_WATER_MARKER_OR_COPY_TO_OUT) {
            publishProgress(0, 1, L"导出完成");
            _taskRunning = false;
        } else {
            CONSOLE_LOG(val("Unknown phase."));
            _taskRunning = false;
        }
    } else {
        EM_ASM(alert("导出失败"));
        _taskRunning = false;
        std::cout << "exit code is " << code << std::endl;
    }
}

void sr::ExportVideoTask::publishProgress(unsigned long current, unsigned long total, std::wstring message) {
    if (_onProgress) {
        _onProgress(current, total, std::move(message));
    }
}
