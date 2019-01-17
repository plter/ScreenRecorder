const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");

const SRC_DIR = __dirname;
const TARGET_FILE_NAME = "SrvReaderTaskImpl";

gulp.task("CompileSrvReaderTaskImpl", shell.task(`emcc \
    main.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/LocalStorageManager.cpp \
    ../../commons/SRGlobals.cpp \
    ../../commons/SRWindows.cpp \
    ../../commons/Res.cpp \
    ../../commons/ElectronGlobals.cpp \
    ../../commons/worker/SRVConfig.cpp \
    ../../commons/worker/SRVWebM.cpp \
    ../../commons/worker/WorkerGlobals.cpp \
    ../../commons/worker/SRVReader.cpp \
    -I/opt/boost_1_69_0 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s FORCE_FILESYSTEM=1 \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s ENVIRONMENT=node \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, `${TARGET_FILE_NAME}.js`)} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
    {cwd: SRC_DIR})
);

gulp.task("BuildSrvReaderTaskImplAndRun", ["CompileSrvReaderTaskImpl"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));