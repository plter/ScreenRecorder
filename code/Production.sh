#!/usr/bin/env bash

cd out
rm -rf app
cd ..
docker run -t --rm -w /src -v `pwd`:/src xtiqin/yunpweb:ScreenRecorderBuilderEnv gulp CopyDebugFilesToApp
cd out/app
npm install --production