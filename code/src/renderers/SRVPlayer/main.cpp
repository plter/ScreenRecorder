//
// Created by yunp on 2018/4/9.
//
#include "../../commons/node_globals.h"
#include "SRVPlayerEntry.h"
#include "../../commons/LocalStorageManager.h"

using namespace emscripten;

int main() {
    sr::NodeGlobals::getInstance()->mountNODEFS(sr::LocalStorageManager::getSRLibraryDirectory());
    sr::SRVPlayerEntry::getInstance();
    return 0;
}