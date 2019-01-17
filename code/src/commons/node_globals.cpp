//
// Created by yunp on 2018/4/5.
//

#include "node_globals.h"
#include <emscripten/bind.h>
#include "SRGlobals.h"

using namespace emscripten;

namespace sr {
    NodeGlobals *NodeGlobals::__instance = nullptr;

    NodeGlobals::NodeGlobals() :
            _path(REQUIRE("path")),
            _os(REQUIRE("os")),
            _url(REQUIRE("url")),
            _fs(REQUIRE("fs")),
            _md5(REQUIRE("js-md5")),
            _child_process(REQUIRE("child_process")) {
    }

    const emscripten::val &NodeGlobals::get_path() const {
        return _path;
    }

    NodeGlobals *NodeGlobals::getInstance() {
        if (!NodeGlobals::__instance) {
            NodeGlobals::__instance = new NodeGlobals();
        }
        return NodeGlobals::__instance;
    }

    const emscripten::val &NodeGlobals::get_os() const {
        return _os;
    }

    const emscripten::val &NodeGlobals::get_url() const {
        return _url;
    }

    const emscripten::val &NodeGlobals::get_fs() const {
        return _fs;
    }

    void NodeGlobals::mountNODEFS(val srLibraryPath) {

        if (!NODE_FS.call<bool>("existsSync", srLibraryPath)) {
            mkdirs(srLibraryPath);
        }

        //Mount /SRLibrary and /SRTmp
        JS_FUNCTION(val("srLib"), val("nodeFsSRLibrary"), val("srTmp"), val(R"(
  const os = require("os");
  FS.mkdir(nodeFsSRLibrary);
  FS.mount(NODEFS, {root:srLib}, nodeFsSRLibrary);
  FS.mkdir(srTmp);
  FS.mount(NODEFS,{root:os.tmpdir()},srTmp);
)"))(srLibraryPath, val(NODEFS_SR_LIBRARY), val(NODEFS_SR_TMP));
    }

    const val &NodeGlobals::get_md5() const {
        return _md5;
    }

    const val &NodeGlobals::get_child_process() const {
        return _child_process;
    }

    void NodeGlobals::mkdirs(emscripten::val dirPath) {
        val parent = NODE_PATH_DIR_NAME(dirPath);
        if (NODE_FS.call<bool>("existsSync", parent)) {
            NODE_FS.call<void>("mkdirSync", dirPath);
        } else {
            mkdirs(parent);
        }
    }
}