//
// Created by yunp on 2018/4/5.
//

#ifndef SCREENRECORDER_NODE_GLOBALS_H
#define SCREENRECORDER_NODE_GLOBALS_H

#define REQUIRE(name) \
    emscripten::val::global("require")(emscripten::val(name))

#define JQUERY emscripten::val::global("jQuery")
#define MD5 sr::NodeGlobals::getInstance()->get_md5()
#define CHILD_PROCESS sr::NodeGlobals::getInstance()->get_child_process()
#define NODE_PATH sr::NodeGlobals::getInstance()->get_path()
#define NODE_OS sr::NodeGlobals::getInstance()->get_os()
#define NODE_URL sr::NodeGlobals::getInstance()->get_url()
#define NODE_FS sr::NodeGlobals::getInstance()->get_fs()
#define NODE_PATH_DIR_NAME(the_path) NODE_PATH.call<emscripten::val>("dirname",emscripten::val(the_path))
#define NODE_PATH_JOIN(args...) NODE_PATH.call<val>("join", args);

#define JS_FUNCTION(args...) emscripten::val::global("Function").new_(args)
#define LAMBDA_TO_FUNCTION_POINTER(args...) (void(*)(args))[](args)

#define JSON emscripten::val::global("JSON")
#define __FILE_NAME emscripten::val::global("__filename")
#define __DIR_NAME emscripten::val::global("__dirname")

#define APP_ROOT NODE_PATH_DIR_NAME(NODE_PATH_DIR_NAME(NODE_PATH_DIR_NAME(__DIR_NAME)))

#define DOCUMENT emscripten::val::global("document")
#define DOCUMENT_CREATE_ELEMENT(element) DOCUMENT.call<emscripten::val>("createElement",emscripten::val(element))

#define DOCUMENT_QUERY_SELECTOR(selector) DOCUMENT.call<emscripten::val>("querySelector",emscripten::val(selector))

#define MAKE_JSON_OBJECT(json_string) \
    JSON.call<emscripten::val>("parse",emscripten::val(json_string))

#define CONSOLE_LOG(VAL...) emscripten::val::global("console").call<void>("log",VAL)

#include <emscripten/bind.h>

namespace sr {
    class NodeGlobals {
    private:
        emscripten::val _path;
        emscripten::val _os;
        emscripten::val _url;
        emscripten::val _fs;
        emscripten::val _md5;
        emscripten::val _child_process;
    public:
        const emscripten::val &get_md5() const;

    public:
        const emscripten::val &get_fs() const;

        const emscripten::val &get_url() const;

        const emscripten::val &get_os() const;

        const emscripten::val &get_child_process() const;

        void mountNODEFS(emscripten::val srLibraryPath);

        void mkdirs(emscripten::val dirPath);

    private:

        NodeGlobals();

        static NodeGlobals *__instance;

    public:
        static NodeGlobals *getInstance();

        const emscripten::val &get_path() const;
    };
}
#endif //SCREENRECORDER_NODE_GLOBALS_H
