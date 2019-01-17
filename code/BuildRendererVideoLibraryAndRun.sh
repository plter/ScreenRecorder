#!/usr/bin/env bash

docker run -t --rm -w /src -v `pwd`:/src xtiqin/yunpweb:ScreenRecorderBuilderEnv gulp CompileRendererVideoLibrary
cd out
cd debug
npm start