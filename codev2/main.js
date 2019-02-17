const electron = require('electron');
const ipcMain = electron.ipcMain;
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const path = require('path');
const globalShortcut = electron.globalShortcut;
const os = require("os");

let Main = {

    run() {
        this.addListeners();

        if (app.isReady()) {
            this.initProperties();
        } else {
            app.on('ready', () => {
                this.initProperties();
            });
        }
    },

    initProperties() {
        this.createTray();
        this.createWindow();
        this.registerGlobalShortcuts();
    },

    createWindow() {
        // Create the browser window.

        let options = {
            width: 280,
            height: 130,
            resizable: false,
            fullscreenable: false,
            title: "ScreenRecorder",
            maximizable: false,
            webPreferences: {
                nodeIntegration: true
            }
        };
        if (os.type() === "Linux") {
            options.icon = path.join(__dirname, "src", "res", "icons", "ScreenRecorder.png");
        }

        this.mainWindow = new BrowserWindow(options);

        // and load the index.html of the app.
        this.mainWindow.loadFile(path.join(__dirname, "src", "renderers", "index", "index.html"));

        // Open the DevTools.
        // this.mainWindow.webContents.openDevTools();

        // Emitted when the window is closed.
        this.mainWindow.on('closed', () => {
            // Dereference the window object, usually you would store windows
            // in an array if your app supports multi windows, this is the time
            // when you should delete the corresponding element.
            this.mainWindow = null;
        });
    },

    createTray() {
        this.tray = new electron.Tray(path.join(__dirname, "src", "res", "images", "icon_16.png"));
        const contextMenu = electron.Menu.buildFromTemplate([
            {
                label: '退出',
                accelerator: "CmdOrCtrl+Q",
                click: (item, focusedWindow) => {
                    app.quit();
                }
            }
        ]);
        this.tray.setToolTip('ScreenRecorder');
        this.tray.setContextMenu(contextMenu);
    },

    addListeners() {
        app.on('window-all-closed', () => {
            app.quit();
        });

        app.on('activate', () => {
            if (this.mainWindow === null) {
                this.createWindow();
            }
        });

        app.on('will-quit', () => {
            this.unregisterAllGlobalShortcuts();
        });


        ipcMain.on("stopped", event => {
            this.tray.setImage(path.join(__dirname, "src", "res", "images", "icon_16.png"));
            event.returnValue = 1;
        });

        ipcMain.on("paused", event => {
            this.tray.setImage(path.join(__dirname, "src", "res", "images", "icon_paused_16.png"));
            event.returnValue = 1;
        });

        ipcMain.on("recording", event => {
            this.tray.setImage(path.join(__dirname, "src", "res", "images", "icon_recording_16.png"));
            event.returnValue = 1;
        });

    },

    registerGlobalShortcuts() {
        //Start or stop button
        globalShortcut.register("CommandOrControl+8", () => {
            this.sendStartOrStopButtonClickedEvent();
        });
        //Pause or resume button
        globalShortcut.register("CommandOrControl+9", () => {
            this.sendPauseOrResumeButtonClickedEvent();
        });
    },

    sendPauseOrResumeButtonClickedEvent() {
        this.mainWindow.webContents.send("pauseOrResume");
    },

    sendStartOrStopButtonClickedEvent() {
        this.mainWindow.webContents.send("startOrStop");
    },

    unregisterAllGlobalShortcuts() {
        globalShortcut.unregisterAll();
    }
};

Main.run();