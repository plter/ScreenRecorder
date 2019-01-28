const Dialogs = require("../commons/Dialogs");
const FlexBoxApplication = require("../commons/FlexBoxApplication");
const LocalStorageManager = require("../commons/LocalStorageManager");
const AudioDeviceSelectorConfig = require("./AudioDeviceSelectorConfig");
const RecordStatus = require("./RecordStatus");
const StreamQueue = require("./StreamQueue");

class Entry extends FlexBoxApplication {

    constructor() {
        super(document.querySelector("#root"));
        this.run();
    }

    async run() {
        this.buildUI();
        this.addListeners();

        this.recordState = RecordStatus.STOPPED;

        await this.initProperties();
    }

    buildUI() {
        this.buildMainMenu();
        this._selectorAudioInput = new window.Vue(AudioDeviceSelectorConfig.create("#select-audio-input-container"));
        this._btnStartOrStop = document.querySelector("#btn-start-or-stop");
        this._btnPauseOrResume = document.querySelector("#btn-pause-or-resume");
    }

    async initProperties() {
        let allDevices = await navigator.mediaDevices.enumerateDevices();
        let audioDevices = [];
        for (let d of allDevices) {
            if (d.kind === "audioinput") {
                audioDevices.push(d);
            }
        }
        this._selectorAudioInput.setDevices(audioDevices);
    }

    addListeners() {
        $("#btn-show-main-menu").click(e => this._mainMenu.popup({x: 8, y: 60}));

        //Global key listener
        window.electron.ipcRenderer.on("startOrStop", this.startOrStopHandler.bind(this));
        window.electron.ipcRenderer.on("pauseOrResume", this.pauseOrResumeHandler.bind(this));
        this._btnPauseOrResume.onclick = this.pauseOrResumeHandler.bind(this);
        this._btnStartOrStop.onclick = this.startOrStopHandler.bind(this);
        $("#btn-show-donate-window").click(e => Dialogs.showDonateDialog());
        $("#btn-show-settings-window").click(e => Dialogs.showSettingsDialog());
        $("#btn-show-video-library").click(e => Dialogs.showVideoLibrary());
    }


    async startOrStopHandler() {
        if (this.recordState == RecordStatus.STOPPED) {
            this.recordState = RecordStatus.RETRIEVING_SCREEN_STREAM;
            let width = screen.width * 2;
            let height = screen.height * 2;
            let _videoStream = await navigator.mediaDevices.getUserMedia({
                audio: false,
                video: {
                    mandatory: {
                        chromeMediaSource: 'desktop',
                        minWidth: width,
                        maxWidth: width,
                        minHeight: height,
                        maxHeight: height
                    }
                }
            });
            if (this._selectorAudioInput.checked) {
                this.recordState = RecordStatus.RETRIEVING_AUDIO_STREAM;
                let audioStream = await navigator.mediaDevices.getUserMedia({
                    video: false,
                    audio: this._selectorAudioInput.getSelectedAudioDevice()
                });
                for (let at of audioStream.getAudioTracks()) {
                    _videoStream.addTrack(at);
                }
            }
            this.recordState = RecordStatus.PENDING_TO_START_RECORD;
            window.electron.remote.getCurrentWindow().minimize();
            Dialogs.showPendingToStartWindow().once("closed", e => {
                this.recordState = RecordStatus.RECORDING;
                let streamQueue = new StreamQueue(window.path.join(LocalStorageManager.getDestDir(), `${Date.now()}.srv`));
                this._currentRecorder = new MediaRecorder(_videoStream, {
                    mimeType: "video/webm;codecs=h264",
                    audioBitsPerSecond: parseInt(LocalStorageManager.getAudioBps()),
                    videoBitsPerSecond: parseInt(LocalStorageManager.getVideoBps())
                });

                this._currentRecorder.ondataavailable = e => {
                    streamQueue.appendData(e.data);
                };
                this._currentRecorder.start(1000);
            });
        } else if (this.recordState == RecordStatus.RECORDING) {
            this._currentRecorder.stop();
            this.recordState = RecordStatus.STOPPED;
            window.electron.remote.getCurrentWindow().restore();
        }
    }


    pauseOrResumeHandler() {
        if (this.recordState == RecordStatus.RECORDING) {
            this.recordState = RecordStatus.PAUSED;
            this._currentRecorder.pause();
        } else if (this.recordState == RecordStatus.PAUSED) {
            this.recordState = RecordStatus.RECORDING;
            this._currentRecorder.resume();
        }
    }

    buildMainMenu() {
        this._mainMenu = new window.electron.remote.Menu();
        // this._mainMenu.append(this.createMenuItem("视频库", () => Dialogs.showVideoLibrary(), window.path.join(window.appPath, "src", "res", "icons", "video_library_18.png")));
        // this._mainMenu.append(this.createMenuItem("设置", () => Dialogs.showSettingsDialog(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_settings_black_18dp.png")));
        // this._mainMenu.append(this.createMenuItem(undefined, undefined, undefined, "separator"));
        // this._mainMenu.append(this.createMenuItem("捐助", () => Dialogs.showDonateDialog(), window.path.join(window.appPath, "src", "res", "icons", "wechat_18.png")));
        this._mainMenu.append(this.createMenuItem("关于 ScreenRecorder", () => Dialogs.showAboutDialog(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_info_outline_black_18dp.png")));
        // this._mainMenu.append(this.createMenuItem(undefined, undefined, undefined, "separator"));
        this._mainMenu.append(this.createMenuItem("退出", () => window.close(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_highlight_off_black_18dp.png")));

    }

    createMenuItem(label, clickCallback, iconPath, type) {
        return new window.electron.remote.MenuItem({
            label: label,
            click: clickCallback,
            type: type,
            icon: iconPath
        });
    }

    set recordState(state) {
        this._recordState = state;
        switch (state) {
            case RecordStatus.STOPPED:
                window.electron.ipcRenderer.send("stopped");
                this._btnStartOrStop.disabled = false;
                this._btnPauseOrResume.disabled = true;
                this._btnStartOrStop.innerHTML = "<span class='fa fa-circle'></span>";
                break;
            case RecordStatus.PAUSED:
                window.electron.ipcRenderer.send("paused");
                this._btnStartOrStop.disabled = true;
                this._btnPauseOrResume.disabled = false;
                this._btnPauseOrResume.innerHTML = "<span class='fa fa-sync'></span>";
                break;
            case RecordStatus.RECORDING:
                window.electron.ipcRenderer.send("recording");
                this._btnStartOrStop.disabled = false;
                this._btnPauseOrResume.disabled = false;
                this._btnPauseOrResume.innerHTML = "<span class='fa fa-pause'></span>";
                this._btnStartOrStop.innerHTML = "<span class='fa fa-stop'></span>";
                break;
        }
    }

    get recordState() {
        return this._recordState;
    }
}

new Entry();