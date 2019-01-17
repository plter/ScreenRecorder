//
// Created by yunp on 2018/4/10.
//

#include "SRGlobals.h"
#include "node_globals.h"

using namespace emscripten;

static void createGlobalFuncs() {
    emscripten_run_script(R"(
(function(top){
  top.sr = top.sr || {
    funcs:{
      newWithArg:function(clazz){
        return new clazz();
      },
      newWithArg1:function(clazz, arg1){
        return new clazz(arg1);
      },
      newWithArg2:function(clazz,arg1,arg2){
        return new clazz(arg1,arg2);
      },
      newWithArg3:function(clazz,arg1,arg2,arg3){
        return new clazz(arg1,arg2,arg3);
      }
    }
  }
})(this);
)");
}


emscripten::val sr::SRGlobals::JavaScriptObjectNew(emscripten::val JavaScriptClass) {
    createGlobalFuncs();
    val newWithArg = val::global("sr")["funcs"]["newWithArg"];
    return newWithArg(JavaScriptClass);
}

emscripten::val
sr::SRGlobals::JavaScriptObjectNew1(emscripten::val JavaScriptClass,
                                    emscripten::val arg1) {
    createGlobalFuncs();
    val newWithArg1 = val::global("sr")["funcs"]["newWithArg1"];
    return newWithArg1(JavaScriptClass, arg1);
}

emscripten::val
sr::SRGlobals::JavaScriptObjectNew2(emscripten::val JavaScriptClass,
                                    emscripten::val arg1,
                                    emscripten::val arg2) {
    createGlobalFuncs();
    val newWithArg2 = val::global("sr")["funcs"]["newWithArg2"];
    return newWithArg2(JavaScriptClass, arg1, arg2);
}

emscripten::val
sr::SRGlobals::JavaScriptObjectNew3(emscripten::val JavaScriptClass,
                                    emscripten::val arg1,
                                    emscripten::val arg2,
                                    emscripten::val arg3) {
    createGlobalFuncs();
    val newWithArg3 = val::global("sr")["funcs"]["newWithArg3"];
    return newWithArg3(JavaScriptClass, arg1, arg2, arg3);
}

emscripten::val sr::SRGlobals::JSFunction(std::string functionBody) {
    return JavaScriptObjectNew1(val::global("Function"), val(functionBody));
}

emscripten::val sr::SRGlobals::JSFunction1(std::string argName1, std::string functionBody) {
    return JavaScriptObjectNew2(val::global("Function"), val(argName1), val(functionBody));
}

emscripten::val sr::SRGlobals::JSFunction2(std::string argName1, std::string argName2, std::string functionBody) {
    return JavaScriptObjectNew3(val::global("Function"), val(argName1), val(argName2), val(functionBody));
}

emscripten::val sr::SRGlobals::JSFunctionWB(std::wstring functionBody) {
    return JavaScriptObjectNew1(val::global("Function"), val(functionBody));
}

emscripten::val sr::SRGlobals::JSFunction1WB(std::string argName1, std::wstring functionBody) {
    return JavaScriptObjectNew2(val::global("Function"), val(argName1), val(functionBody));
}
