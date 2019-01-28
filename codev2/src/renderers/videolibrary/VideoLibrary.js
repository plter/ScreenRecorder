const FlexBoxApplication = require("../commons/FlexBoxApplication");
const SrvListConfig = require("./SrvListConfig");


class Entry extends FlexBoxApplication {

    constructor() {
        super(document.querySelector("#root"));
        this._srvList = new window.Vue(SrvListConfig.create("#srv-list"));
    }
}

new Entry();