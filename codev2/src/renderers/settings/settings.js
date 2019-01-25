const LocalStorageManager = require("../commons/LocalStorageManager");
const Dialogs = require("../commons/Dialogs");

const Entry = {
    run() {
        this.buildUI();
        this.addListeners();
    },

    buildUI() {
        this._rootContainer = document.querySelector("#root");
        this._audioBpsInput = document.querySelector("#audioBpsInput");
        this._audioBpsInput.value = LocalStorageManager.getAudioBps();
        this._videoBpsInput = document.querySelector("#videoBpsInput");
        this._videoBpsInput.value = LocalStorageManager.getVideoBps();
        this._dirPathInput = document.querySelector("#destDirPath");
        this._dirPathInput.value = LocalStorageManager.getDestDir();

        this.resizeRootByWindowSize();
    },

    resizeRootByWindowSize() {
        this._rootContainer.style.height = `${window.innerHeight}px`;
    },

    btnBrowserForDistDirClickedHandler(e) {
        let paths = window.electron.remote.dialog.showOpenDialog(
            window.electron.remote.getCurrentWindow(),
            {title: "选择视频保存目录", properties: ['openDirectory', 'createDirectory'], defaultPath: this._dirPathInput.value}
        );
        if (paths && paths.length) {
            let firstPath = paths[0];
            this._dirPathInput.value = firstPath;
            LocalStorageManager.setDestDir(firstPath);
        }
    },

    _videoBpsInputChangeHandler() {
        LocalStorageManager.setVideoBps(this._videoBpsInput.value);
    },

    _audioBpsInputChangeHandler() {
        LocalStorageManager.setAudioBps(this._audioBpsInput.value);
    },

    addListeners() {
        window.onresize = this.resizeRootByWindowSize.bind(this);
        $("#btn-open-in-file").click(e => window.electron.shell.showItemInFolder(this._dirPathInput.value));
        $("#btnBrowserForDistDir").click(this.btnBrowserForDistDirClickedHandler.bind(this));
        this._videoBpsInput.onchange = this._videoBpsInputChangeHandler.bind(this);
        this._audioBpsInput.onchange = this._audioBpsInputChangeHandler.bind(this);
        $("#btn-open-video-library").click(e => Dialogs.showVideoLibrary());
    }
};

Entry.run();