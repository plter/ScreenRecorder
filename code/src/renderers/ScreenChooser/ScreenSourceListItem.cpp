//
// Created by yunp on 2018/4/8.
//

#include "ScreenSourceListItem.h"
#include "../../commons/node_globals.h"
#include <boost/format.hpp>
#include <iostream>
#include "../../commons/LocalStorageManager.h"

using namespace emscripten;
namespace sr {
    ScreenSourceListItem::ScreenSourceListItem(const emscripten::val &_screenSource) :
            _screenSource(_screenSource),
            _node(DOCUMENT.call<emscripten::val>("createElement", val("div"))),
            _jq(JQUERY(_node)) {

        buildUI();
        addListeners();
    }

    void ScreenSourceListItem::buildUI() {
        _jq.call<void>("addClass", val("screen-source-item"));
        _jq.call<void>("html", (boost::format(R"(
<div class="screen-source-item-thumbnail"><img src=""></div>
<div class="screen-source-item-info">
  <div class="screen-source-item-name"></div>
  <div class="screen-source-item-id"></div>
</div>)").str()));

        _jq.call<val>("find", val(".screen-source-item-thumbnail img")).call<void>("attr", val("src"),
                                                                                   _screenSource["thumbnail"].call<val>(
                                                                                           "toDataURL"));
        _jq.call<val>("find", val(".screen-source-item-name")).call<void>("html", _screenSource["name"]);
        _jq.call<val>("find", val(".screen-source-item-id")).call<void>("html", _screenSource["id"]);
    }

    const val &ScreenSourceListItem::get_node() const {
        return _node;
    }

    EMSCRIPTEN_BINDINGS(ScreenChooser_ScreenSourceListItem_clickedHandler) {
        function("ScreenChooser_ScreenSourceListItem_clickedHandler",
                 (void (*)(unsigned long)) [](unsigned long __this) {
                     reinterpret_cast<ScreenSourceListItem *>(__this)->onClickedHandler();
                 });
    }

    void ScreenSourceListItem::addListeners() {
        _jq.call<void>("click",
                       JS_FUNCTION((boost::format("Module.ScreenChooser_ScreenSourceListItem_clickedHandler(%1%)") %
                                    (unsigned long) this).str()));
    }

    void ScreenSourceListItem::onClickedHandler() {
        LocalStorageManager::setSelectedScreenStreamID(_screenSource["id"]);
        val::global("window").call<void>("close");
    }
}