//
// Created by yunp on 2018/4/9.
//

#include "../../src/commons/worker/SRVReader.h"
#include <stdio.h>

int main() {
    sr::SRVReader reader("/Users/yunp/SRLibrary/20180417122726.srv");
    reader.read();

    std::stringbuf buf;
    reader.getFirstSRVWebM()->concatParts(buf);

    auto size = buf.in_avail();
//    printf("%d\n", size);
    char *data = new char[size];
    buf.sgetn(data, size);

    FILE *f = fopen("/Users/yunp/Desktop/data.webm", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    } else {
        printf("Can not write file\n");
    }
    delete[]data;
//
//    FILE * pFile;
//    char buffer[] = { 'x' , 'y' , 'z' };
//    pFile = fopen ("/Users/yunp/Desktop/data1.bin", "wb");
//    fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
//    fclose (pFile);
    return 0;
}