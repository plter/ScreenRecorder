const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");


gulp.task("CopyRendererVideoLibraryHTML", function () {
    gulp.src(path.join(__dirname, "VideoLibrary.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererVideoLibrary", ["CopyRendererVideoLibraryHTML"], shell.task(`emcc \
    main.cpp \
    VideoLibraryEntry.cpp \
    ExportVideoTask.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/LocalStorageManager.cpp \
    ../../commons/SRGlobals.cpp \
    ../../commons/SRWindows.cpp \
    ../../commons/Res.cpp \
    ../../commons/SRVReaderTask.cpp \
    ../../commons/ElectronGlobals.cpp \
    ../../commons/worker/SRVWebM.cpp \
    ../../commons/worker/SRVConfig.cpp \
    ../../commons/worker/SRVReader.cpp \
    ../../commons/worker/WorkerGlobals.cpp \
    VideoFileListItem.cpp \
    -I/opt/boost_1_69_0 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s ENVIRONMENT=node \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "VideoLibrary.js")} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
    {cwd: __dirname})
);

gulp.task("BuildRendererVideoLibraryAndRun", ["CompileRendererVideoLibrary"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));