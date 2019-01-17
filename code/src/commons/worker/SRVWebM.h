//
// Created by yunp on 2018/4/16.
//

#ifndef SCREENRECORDER_SRVWEBM_H
#define SCREENRECORDER_SRVWEBM_H

#include <string>
#include <sstream>
#include <vector>

namespace sr {
    class SRVWebM {
    private:
        std::string _name;
        std::vector<std::stringbuf> _webmParts;
    public:
        const std::string &get_name() const;

        void set_name(const std::string &_name);

        void addWebMPart(char *data, size_t dataSize);

        void concatParts(std::stringbuf &output);

        /**
         * 获取视频长度，单位是毫秒，可能不准确，该值主要是用于呈现进度条的参考值
         * @return {unsigned long}
         */
        unsigned long getSrvDuration();

        void writeTo(std::string &outFilePath);

    public:
        explicit SRVWebM(std::string name);
    };
}

#endif //SCREENRECORDER_SRVWEBM_H
