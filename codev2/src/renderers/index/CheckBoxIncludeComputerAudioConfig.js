const LocalStorageManager = require("../commons/LocalStorageManager");

const CheckBoxIncludeComputerAudioConfig = {
    create(el) {
        return {
            el: el,
            data: {checked: LocalStorageManager.getIfIncludeComputerAudio() === "yes"},
            watch: {
                checked: function (value, oldValue) {
                    LocalStorageManager.setIfIncludeComputerAudio(value ? "yes" : "no");
                }
            }
        };
    }
};

module.exports = CheckBoxIncludeComputerAudioConfig;