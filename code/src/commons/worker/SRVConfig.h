//
// Created by yunp on 2018/4/16.
//

#ifndef SCREENRECORDER_SRVCONFIG_H
#define SCREENRECORDER_SRVCONFIG_H


#include <sstream>

namespace sr {
    class SRVConfig {
    public:
        SRVConfig(char *data, uint32_t dataSize);

    private:
        std::stringbuf _configContent;
    };
}

#endif //SCREENRECORDER_SRVCONFIG_H
