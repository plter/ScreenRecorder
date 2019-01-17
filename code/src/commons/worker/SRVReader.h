//
// Created by yunp on 2018/4/15.
//

#ifndef SCREENRECORDER_SRVREADER_H
#define SCREENRECORDER_SRVREADER_H


#include <iostream>
#include <vector>
#include <memory>
#include "SRVWebM.h"
#include "SRVConfig.h"
#include <map>
#include <emscripten/bind.h>

namespace sr {
    class SRVFileNotFoundException : public std::exception {
    };

    class SRVUnknownTypeException : public std::exception {
    };

    class SRVFileDamagedException : public std::exception {
    };

    class SRVReader {
    private:
        std::string _filePath;
        std::map<std::string, std::shared_ptr<SRVWebM>> _srvWebMFiles;
        std::shared_ptr<SRVConfig> _srvConfig;

    public:
        explicit SRVReader(const std::string &_filePath);

        void read();
        bool hasSRVWebM();

        std::shared_ptr<SRVWebM> getFirstSRVWebM();
    };
}


#endif //SCREENRECORDER_SRVREADER_H
