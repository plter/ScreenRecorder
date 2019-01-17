
#include "Entry.h"
#include "../../commons/node_globals.h"
#include "../../commons/LocalStorageManager.h"

int main() {
    sr::NodeGlobals::getInstance()->mountNODEFS(sr::LocalStorageManager::getSRLibraryDirectory());

    new sr::Entry();
    return 0;
}