// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const Dialogs = require("../commons/Dialogs");

const Entry = {
    run() {
        this.buildUI();
        this.addListeners();
    },

    buildUI() {
        this.buildMainMenu();
        this._rootContainer = document.querySelector("#root");
        this.resizeRootByWindowSize();
    },

    resizeRootByWindowSize() {
        this._rootContainer.style.height = `${window.innerHeight}px`;
    },

    addListeners() {
        $("#btn-show-main-menu").click(e => this._mainMenu.popup({x: 8, y: 60}));
        window.onresize = this.resizeRootByWindowSize.bind(this);
    },

    buildMainMenu() {
        this._mainMenu = new window.electron.remote.Menu();
        this._mainMenu.append(this.createMenuItem("视频库", () => Dialogs.showVideoLibrary(), window.path.join(window.appPath, "src", "res", "icons", "video_library_18.png")));
        this._mainMenu.append(this.createMenuItem("设置", () => Dialogs.showSettingsDialog(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_settings_black_18dp.png")));
        this._mainMenu.append(this.createMenuItem(undefined, undefined, undefined, "separator"));
        this._mainMenu.append(this.createMenuItem("捐助", () => Dialogs.showDonateDialog(), window.path.join(window.appPath, "src", "res", "icons", "wechat_18.png")));
        this._mainMenu.append(this.createMenuItem("关于 ScreenRecorder", () => Dialogs.showAboutDialog(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_info_outline_black_18dp.png")));
        this._mainMenu.append(this.createMenuItem(undefined, undefined, undefined, "separator"));
        this._mainMenu.append(this.createMenuItem("退出", () => window.close(), window.path.join(window.appPath, "src", "res", "icons", "MaterialDesignIcons", "ic_highlight_off_black_18dp.png")));

    },

    createMenuItem(label, clickCallback, iconPath, type) {
        return new window.electron.remote.MenuItem({
            label: label,
            click: clickCallback,
            type: type,
            icon: iconPath
        });
    }
};

Entry.run();