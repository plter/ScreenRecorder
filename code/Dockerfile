FROM xtiqin/emsdk
RUN apt update
RUN apt install -y p7zip-full
RUN apt install -y wget
WORKDIR /opt
RUN wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.bz2
RUN 7z x boost_1_69_0.tar.bz2
RUN 7z x boost_1_69_0.tar
RUN cnpm install -g gulp