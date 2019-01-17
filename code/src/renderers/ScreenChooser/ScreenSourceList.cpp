//
// Created by yunp on 2018/4/8.
//

#include "ScreenSourceList.h"
#include "../../commons/node_globals.h"
#include "ScreenSourceListItem.h"
#include <memory>

namespace sr {
    ScreenSourceList::ScreenSourceList(emscripten::val sources) : _sources(sources),
                                                                  _container(DOCUMENT_QUERY_SELECTOR("#container")) {
        initUI();
    }

    void ScreenSourceList::initUI() {
        _container.set("innerHTML", std::string(""));
        auto length = _sources["length"].as<int>();
        for (int i = 0; i < length; i++) {
            auto item = std::make_shared<ScreenSourceListItem>(_sources[i]);
            _items.push_back(item);
            _container.call<void>("appendChild", item->get_node());
        }
    }

}