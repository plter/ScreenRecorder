
#include "../../commons/ElectronGlobals.h"
#include "../../commons/node_globals.h"
#include <boost/format.hpp>

using namespace emscripten;

int main() {
    val versionDiv = DOCUMENT_QUERY_SELECTOR("#version");
    versionDiv.set("innerHTML",
                   (boost::wformat(LR"(版本：v%1%)") %
                    sr::ElectronGlobals::getInstance()->getAppVersion().as<std::wstring>()).str());
    return 0;
}
