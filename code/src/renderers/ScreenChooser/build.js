const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");


gulp.task("CopyRendererScreenChooserHTML", function () {
    gulp.src(path.join(__dirname, "ScreenChooser.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererScreenChooser", ["CopyRendererScreenChooserHTML"], shell.task(`emcc \
    main.cpp \
    ScreenChooserEntry.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/LocalStorageManager.cpp \
    ../../commons/ElectronGlobals.cpp \
    ScreenSourceList.cpp \
    ScreenSourceListItem.cpp \
    -I/opt/boost_1_69_0 \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s DEMANGLE_SUPPORT=1 \
    -s ENVIRONMENT=node \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "ScreenChooser.js")} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
    {cwd: __dirname}
));