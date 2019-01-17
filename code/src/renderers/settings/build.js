const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");

gulp.task("CopyRendererSettingsHTML", function () {
    gulp.src("src/renderers/settings/settings.html").pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererSettings",
    ['CopyRendererSettingsHTML'],
    shell.task(`emcc \
    main.cpp \
    SettingsEntry.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/LocalStorageManager.cpp \
    ../../commons/SRWindows.cpp \
    ../../commons/SRGlobals.cpp \
    ../../commons/ElectronGlobals.cpp \
    -I/opt/boost_1_69_0 \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s DEMANGLE_SUPPORT=1 \
    -s ENVIRONMENT=node \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "settings.js")} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
        {cwd: __dirname}
    ));

gulp.task("BuildRendererSettingsAndRun", ["CompileRendererSettings"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));