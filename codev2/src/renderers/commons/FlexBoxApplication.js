class FlexBoxApplication {

    constructor(rootElement) {
        this._rootElement = rootElement;
        this._flexBoxApplicationWindowChangeHandler();
    }

    _flexBoxApplicationAddListeners() {
        window.addEventListener("change", this._flexBoxApplicationWindowChangeHandler.bind(this));
    }

    _flexBoxApplicationWindowChangeHandler(e) {
        this.rootElement.style.height = `${window.innerHeight}px`;
    }

    get rootElement() {
        return this._rootElement;
    }
}

module.exports = FlexBoxApplication;