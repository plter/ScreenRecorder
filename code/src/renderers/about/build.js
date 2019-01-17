const gulp = require("gulp");
const path = require("path");
const shell = require("gulp-shell");


gulp.task("CopyRendererAboutHTML", function () {
    gulp.src(path.join(__dirname, "about.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});

gulp.task("CompileRendererAbout", ["CopyRendererAboutHTML"], shell.task(`emcc \
    main.cpp \
    ../../commons/node_globals.cpp \
    ../../commons/SRGlobals.cpp \
    ../../commons/ElectronGlobals.cpp \
    -s WASM=1 \
    -s ASSERTIONS=1 \
    -s DEMANGLE_SUPPORT=1 \
    -s ENVIRONMENT=node \
    -I/opt/boost_1_69_0 \
    -o ${path.join(RENDERERS_DEBUG_OUTPUT_DIR, "about.js")} \
    -std=c++11 -Wall --bind ${COMPILE_LEVEL}`,
    {cwd: __dirname}
));

gulp.task("BuildRendererAboutAndRun", ["CompileRendererAbout"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));