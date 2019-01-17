//
// Created by yunp on 2018/4/8.
//

#include "ScreenChooserEntry.h"
#include "../../commons/node_globals.h"
#include <emscripten.h>
#include <boost/format.hpp>
#include <utility>
#include "../../commons/ElectronGlobals.h"

using namespace emscripten;

namespace sr {

    ScreenChooserEntry *ScreenChooserEntry::__instance = nullptr;

    ScreenChooserEntry *ScreenChooserEntry::getInstance() {
        if (__instance == nullptr) {
            __instance = new ScreenChooserEntry();
        }
        return __instance;
    }

    ScreenChooserEntry::ScreenChooserEntry() :
            _container(DOCUMENT_QUERY_SELECTOR("#container")),
            _screenSourceList(nullptr) {
        searchStreams();
    }

    EMSCRIPTEN_BINDINGS(ScreenChooserEntry_onGotScreenStreamsSources) {
        function("ScreenChooserEntry_onGotScreenStreamsSources", (void (*)(val, val)) [](val __this, val sources) {
            auto self = reinterpret_cast<ScreenChooserEntry *>(__this.as<unsigned long>());
            self->onGotScreenStreamSourcesHandler(sources);
        });
    }

    void ScreenChooserEntry::searchStreams() {
        ELECTRON_DESKTOP_CAPTURER.call<void>("getSources", MAKE_JSON_OBJECT(R"({"types":["window","screen"]})"),
                                             JS_FUNCTION(val("error"), val("sources"), val((boost::format(R"(
if(error){
  alert("Can not get screen sources,may be your os is not supported!");
}else{
  Module.ScreenChooserEntry_onGotScreenStreamsSources(%1%,sources);
}
)") % (unsigned long) this).str())));
    }

    void ScreenChooserEntry::onGotScreenStreamSourcesHandler(emscripten::val sources) {
        _screenSourceList = new ScreenSourceList(std::move(sources));
    }

    ScreenChooserEntry::~ScreenChooserEntry() {
        delete _screenSourceList;
    }
}