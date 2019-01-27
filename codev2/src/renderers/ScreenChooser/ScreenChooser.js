const FlexBoxApplication = require("../commons/FlexBoxApplication");
const LocalStorageManager = require("../commons/LocalStorageManager");

class ScreenChooser extends FlexBoxApplication {


    constructor() {
        super(document.querySelector("#root"));
        LocalStorageManager.clearSelectedScreen();
        console.log(window.electron.remote.getCurrentWindow());
    }
}

new ScreenChooser();