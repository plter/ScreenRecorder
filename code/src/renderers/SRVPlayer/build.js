const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");


gulp.task("CopyRendererSRVPlayerHTML", function () {
    gulp.src(path.join(__dirname, "SRVPlayer.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererSRVPlayer", ["CopyRendererSRVPlayerHTML"], shell.task(`emcc \
    main.cpp \
    SRVPlayerEntry.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/LocalStorageManager.cpp \
    ../../commons/SRGlobals.cpp \
    ../../commons/md5.cpp \
    ../../commons/worker/SRVReader.cpp \
    ../../commons/worker/SRVWebM.cpp \
    ../../commons/worker/SRVConfig.cpp \
    ../../commons/worker/WorkerGlobals.cpp \
    -I/opt/boost_1_69_0 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s DEMANGLE_SUPPORT=1 \
    -s ENVIRONMENT=node \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "SRVPlayer.js")} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
    {cwd: __dirname}));

gulp.task("BuildRendererSRVPlayerAndRun", ["CompileRendererSRVPlayer"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));