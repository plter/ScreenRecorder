//
// Created by yunp on 2018/4/9.
//

#include "VideoLibraryEntry.h"
#include <emscripten.h>
#include "../../commons/node_globals.h"
#include "../../commons/LocalStorageManager.h"
#include <unistd.h>

using namespace emscripten;


int main() {
    sr::NodeGlobals::getInstance()->mountNODEFS(sr::LocalStorageManager::getSRLibraryDirectory());
    sr::VideoLibraryEntry::getInstance();
    return 0;
}