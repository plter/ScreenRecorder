const LocalStorageManager = require("../commons/LocalStorageManager");

const AudioDeviceSelectorConfig = {
    create(el) {
        return {
            el: el,
            data: {
                options: [],
                selected: undefined,
                optionsMap: null,
                checked: LocalStorageManager.getIfIncludeAudio() === "yes"
            },
            watch: {
                checked: function (value, oldValue) {
                    LocalStorageManager.setIfIncludeAudio(value ? "yes" : "no");
                }
            },
            methods: {
                selectChangeHandler(e) {
                    LocalStorageManager.setSelectedAudioDeviceId(e.target.value);
                },
                setDevices(devices) {
                    this.options = devices;
                    this.optionsMap = new Map();
                    for (let d of devices) {
                        this.optionsMap.set(d.deviceId, d);
                    }

                    let firstDevice = devices[0];
                    this.selected = firstDevice.deviceId;
                    let storedSelectedDeviceId = LocalStorageManager.getSelectedAudioDeviceId();
                    if (storedSelectedDeviceId) {
                        let storedDevice = this.optionsMap.get(storedSelectedDeviceId);
                        if (storedDevice) {
                            this.selected = storedSelectedDeviceId;
                        }
                    }
                },
                getSelectedAudioDevice() {
                    return this.optionsMap.get(this.selected);
                }
            }
        }
    }
};

module.exports = AudioDeviceSelectorConfig;