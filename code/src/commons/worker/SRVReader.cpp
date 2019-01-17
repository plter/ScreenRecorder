//
// Created by yunp on 2018/4/15.
//

#include "SRVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../md5.h"
#include "../SRGlobals.h"
#include <cfloat>
#include "WorkerGlobals.h"

using namespace emscripten;

namespace sr {
    SRVReader::SRVReader(const std::string &_filePath) : _filePath(_filePath) {
    }

    void SRVReader::read() {
        std::ifstream srvIs(_filePath, std::ifstream::binary);
        srvIs.seekg(0, srvIs.end);
        int64_t length = srvIs.tellg();
        srvIs.seekg(0, srvIs.beg);
        if (srvIs) {

            std::shared_ptr<SRVWebM> currentSrvWebM = std::make_shared<SRVWebM>(std::string("Default"));
            while (true) {
                uint8_t dataType;
                srvIs.read(reinterpret_cast<char *>(&dataType), 1);
                if (srvIs.eof()) {
                    if (_srvWebMFiles.count(currentSrvWebM->get_name()) <= 0) {
                        _srvWebMFiles[currentSrvWebM->get_name()] = currentSrvWebM;
                    }
                    break;
                }
                if (dataType == SRV_DATA_TYPE_WEBM_BLOB) {//webm 片断
                    uint32_t webmBlobSize;
                    srvIs.read(reinterpret_cast<char *>(&webmBlobSize), 4);//read webm blob size
//                    printf("size is %d\n", webmBlobSize);
                    char webmBlobBuf[webmBlobSize];
                    srvIs.read(webmBlobBuf, webmBlobSize);//read webm blob data
                    currentSrvWebM->addWebMPart(webmBlobBuf, webmBlobSize);

                    WorkerGlobals::postProgress("reading", L"正在读取文件", static_cast<unsigned long>(srvIs.tellg()),
                                                static_cast<unsigned long>(length));
                } else if (dataType == SRV_DATA_TYPE_WEBM_START) {//webm 头
                    uint32_t nameSize;
                    srvIs.read(reinterpret_cast<char *>(&nameSize), 4);
                    char nameBuf[nameSize];
                    srvIs.read(nameBuf, nameSize);
                    currentSrvWebM = std::make_shared<SRVWebM>(std::string(nameBuf));
                } else if (dataType == SRV_DATA_TYPE_WEBM_END) {//webm 尾
                    _srvWebMFiles[currentSrvWebM->get_name()] = currentSrvWebM;
                } else if (dataType == SRV_DATA_TYPE_CONFIG_FILE) {//config 文件
                    uint32_t configFileSize;
                    srvIs.read(reinterpret_cast<char *>(&configFileSize), 4);
                    char configFileBuf[configFileSize];
                    srvIs.read(configFileBuf, configFileSize);
                    _srvConfig = std::make_shared<SRVConfig>((char *) configFileBuf, configFileSize);
                } else {
                    throw SRVUnknownTypeException();
                }
            }

            WorkerGlobals::postProgress("reading", L"正在读取文件", static_cast<unsigned long>(length),
                                        static_cast<unsigned long>(length));//Reading 100%
            srvIs.close();
        } else {
            throw SRVFileNotFoundException();
        }
    }

    bool SRVReader::hasSRVWebM() {
        return !_srvWebMFiles.empty();
    }

    std::shared_ptr<SRVWebM> SRVReader::getFirstSRVWebM() {
        return (*_srvWebMFiles.begin()).second;
    }
}