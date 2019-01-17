const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");

gulp.task("CopyRendererIndexHTML", function () {
    gulp.src(path.join(__dirname, "index.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererIndex",
    ['CopyRendererIndexHTML'],
    shell.task(
        `emcc \
        main.cpp \
        ../../commons/node_globals.cpp \
        Entry.cpp \
        ../../commons/LocalStorageManager.cpp \
        ../../commons/SRWindows.cpp \
        ../../commons/worker/SRVReader.cpp \
        ../../commons/worker/SRVWebM.cpp \
        ../../commons/md5.cpp \
        ../../commons/SRGlobals.cpp \
        ../../commons/ElectronGlobals.cpp \
        SelectAudioInput.cpp \
        StreamQueue.cpp \
        -I/opt/boost_1_69_0 \
        -s WASM=1 \
        -s ASSERTIONS=1 \
        -s DEMANGLE_SUPPORT=1 \
        -s ENVIRONMENT=node \
        -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "index.js")} \
        -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
        {cwd: __dirname}
    )
);

gulp.task("BuildRendererIndexAndRun", ["CompileRendererIndex"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));
