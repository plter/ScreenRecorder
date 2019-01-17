//
// Created by yunp on 2018/4/16.
//

#include "SRVWebM.h"
#include <utility>
#include <emscripten.h>
#include <fstream>
#include "WorkerGlobals.h"

namespace sr {
    SRVWebM::SRVWebM(std::string name) : _name(std::move(name)) {
    }

    const std::string &SRVWebM::get_name() const {
        return _name;
    }

    void SRVWebM::set_name(const std::string &_name) {
        SRVWebM::_name = _name;
    }

    void SRVWebM::addWebMPart(char *data, size_t dataSize) {
        std::stringbuf buf;
        buf.sputn(data, dataSize);
        buf.pubseekpos(0);
        _webmParts.push_back(std::move(buf));
    }

    void SRVWebM::concatParts(std::stringbuf &output) {
        for (auto &i: _webmParts) {
            i.pubseekpos(0);
            auto size = i.in_avail();
            auto buf = new char[size];
            i.sgetn(buf, size);
            output.sputn(buf, size);
            i.pubseekpos(0);
            delete[](buf);
        }
        output.pubseekpos(0);
    }

    unsigned long SRVWebM::getSrvDuration() {
        return _webmParts.size() * 100;
    }

    void SRVWebM::writeTo(std::string &outFilePath) {
        std::ofstream out(outFilePath);
        if (out) {
            unsigned long count = 0;
            unsigned long total = _webmParts.size();
            for (auto &i: _webmParts) {
                i.pubseekpos(0);
                auto size = i.in_avail();
                auto buf = new char[size];
                i.sgetn(buf, size);
                out.write(buf, size);
                i.pubseekpos(0);
                delete[](buf);

                count++;
                WorkerGlobals::postProgress("writing", L"正在准备文件", count, total);
            }
            out.close();
        } else {
            EM_ASM(alert("Can not write tmp file"));
        }
    }
}
