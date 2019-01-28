const FlexBoxApplication = require("../commons/FlexBoxApplication");
const SrvListConfig = require("./SrvListConfig");
const SrvListItemComponent = require("./SrvListItemComponent");


class Entry extends FlexBoxApplication {

    constructor() {
        super(document.querySelector("#root"));

        this.buildUI();
    }

    buildUI() {
        window.Vue.component("srv-file-list-item", SrvListItemComponent.create());
        this._srvList = new window.Vue(SrvListConfig.create("#srv-list"));
    }
}

new Entry();