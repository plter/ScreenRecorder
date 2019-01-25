/**
 * Created by plter on 3/25/17.
 */

const LS_KEY_BASE_NS = "topyunp_screenrecorder";

const Tools = require("../commons/Tools");

const LocalStorageManager = {
    getDestDir: function () {
        let dest = localStorage.getItem(`${LS_KEY_BASE_NS}_destDir`) || window.path.join(window.electron.remote.app.getPath("home"), "SRLibrary");
        Tools.mkdirs(dest);
        return dest;
    },
    setDestDir: function (value) {
        localStorage.setItem(`${LS_KEY_BASE_NS}_destDir`, value);
    },
    getAudioBps: function () {
        return localStorage.getItem(`${LS_KEY_BASE_NS}_audioBps`) || "128000";
    },
    setAudioBps: function (value) {
        localStorage.setItem(`${LS_KEY_BASE_NS}_audioBps`, value);
    },
    getVideoBps: function () {
        return localStorage.getItem(`${LS_KEY_BASE_NS}_videoBps`) || "6000000";
    },
    setVideoBps: function (value) {
        localStorage.setItem(`${LS_KEY_BASE_NS}_videoBps`, value);
    }
};

module.exports = LocalStorageManager;