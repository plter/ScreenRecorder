const FlexBoxApplication = require("../commons/FlexBoxApplication");


class Entry extends FlexBoxApplication {

    constructor() {
        super(document.querySelector("#root"));
    }
}

new Entry();