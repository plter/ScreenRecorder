#!/usr/bin/env bash

docker run -t --rm -w /src -v `pwd`:/src xtiqin/yunpweb:ScreenRecorderBuilderEnv gulp
cd out
cd debug
cnpm install
npm start