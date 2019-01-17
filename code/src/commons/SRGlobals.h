//
// Created by yunp on 2018/4/10.
//

#pragma once

#define SRV_FILE_TYPE ".srv"
#define SRV_WEBM_BLOB_DURATION 100
#define NODEFS_SR_LIBRARY "/SRLibrary"
#define NODEFS_SR_TMP "/SRTmp"

#include <emscripten.h>
#include <emscripten/val.h>
#include <iostream>

#define SRV_DATA_TYPE_WEBM_BLOB 1
#define SRV_DATA_TYPE_WEBM_START 2
#define SRV_DATA_TYPE_WEBM_END 3
#define SRV_DATA_TYPE_CONFIG_FILE 4


namespace sr {
    class SRGlobals {

    public:

        static emscripten::val
        JavaScriptObjectNew(emscripten::val JavaScriptClass);

        static emscripten::val
        JavaScriptObjectNew1(emscripten::val JavaScriptClass,
                             emscripten::val arg1);

        static emscripten::val
        JavaScriptObjectNew2(emscripten::val JavaScriptClass,
                             emscripten::val arg1,
                             emscripten::val arg2);

        static emscripten::val
        JavaScriptObjectNew3(emscripten::val JavaScriptClass,
                             emscripten::val arg1,
                             emscripten::val arg2,
                             emscripten::val arg3);

        static emscripten::val
        JSFunction(std::string functionBody);

        static emscripten::val
        JSFunctionWB(std::wstring functionBody);

        static emscripten::val
        JSFunction1(std::string argName1, std::string functionBody);

        static emscripten::val
        JSFunction1WB(std::string argName1, std::wstring functionBody);

        static emscripten::val
        JSFunction2(std::string argName1, std::string argName2, std::string functionBody);
    };
}

