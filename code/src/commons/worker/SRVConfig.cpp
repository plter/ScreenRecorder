//
// Created by yunp on 2018/4/16.
//

#include "SRVConfig.h"

namespace sr {
    SRVConfig::SRVConfig(char *data, uint32_t dataSize) {
        _configContent.sputn(data, dataSize);
        _configContent.pubseekpos(0);
    }
}