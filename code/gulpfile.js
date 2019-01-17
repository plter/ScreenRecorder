const gulp = require("gulp");
const shell = require("gulp-shell");
const path = require("path");

global["DEBUG_OUTPUT_DIR"] = path.join(__dirname, "out", "debug");
global["COMPILE_LEVEL"] = "";//-O1,-O2,-O3
global["RENDERERS_DEBUG_OUTPUT_DIR"] = path.join(DEBUG_OUTPUT_DIR, "src", "renderers", "files");
global["RELEASE_OUTPUT_DIR"] = path.join(__dirname, "out", "app");


gulp.task("CopyMainFiles", ["CopyLibFiles"], function () {
    gulp.src("src/main/main.js").pipe(gulp.dest(DEBUG_OUTPUT_DIR));
    gulp.src("package.json").pipe(gulp.dest(DEBUG_OUTPUT_DIR));
});

gulp.task("CopyLibFiles", function () {
    gulp.src("src/lib/**/*").pipe(gulp.dest(path.join(DEBUG_OUTPUT_DIR, "src", "lib")));
});

require("./src/renderers/index/build");
require("./src/renderers/about/build");
require("./src/renderers/VideoLibrary/build");
require("./src/renderers/SrvReaderTaskImpl/build");
require("./src/renderers/settings/build");
require("./src/renderers/ScreenChooser/build");
require("./src/renderers/PendingToStart/build");
require("./src/renderers/SRVPlayer/build");

gulp.task("CopyResFiles", function () {
    gulp.src("res/**/*").pipe(gulp.dest(path.join(DEBUG_OUTPUT_DIR, "res")));
});

gulp.task("CopyDebugFilesToApp", ['BuildDebug'], function () {
    gulp.src(path.join(DEBUG_OUTPUT_DIR, "src/**/*")).pipe(gulp.dest(path.join(RELEASE_OUTPUT_DIR, "src")));
    gulp.src(path.join(DEBUG_OUTPUT_DIR, "res/**/*")).pipe(gulp.dest(path.join(RELEASE_OUTPUT_DIR, "res")));
    gulp.src(path.join(DEBUG_OUTPUT_DIR, "main.js")).pipe(gulp.dest(RELEASE_OUTPUT_DIR));
    gulp.src(path.join(DEBUG_OUTPUT_DIR, "package.json")).pipe(gulp.dest(RELEASE_OUTPUT_DIR));
});


gulp.task("RunDebug", shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));
gulp.task("CNpmInstall", ["BuildDebug"], shell.task("cnpm install", {cwd: DEBUG_OUTPUT_DIR}));
gulp.task("BuildAndRun", ["BuildDebug"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));
gulp.task("BuildAndInstallAndRun", ["CNpmInstall"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));
gulp.task("production", ["CopyDebugFilesToApp"], shell.task("npm install --production", {cwd: RELEASE_OUTPUT_DIR}));

gulp.task("BuildDebug", [
    "CopyMainFiles",
    "CopyResFiles",
    "CopyRendererPendingToStartHTML",
    "CopyRendererAboutHTML",
    "CompileRendererIndex",
    "CompileRendererSettings",
    "CompileRendererScreenChooser",
    "CompileRendererVideoLibrary",
    "CompileRendererSRVPlayer",
    "CompileRendererAbout",
    "CompileSrvReaderTaskImpl"
]);

gulp.task("BuildMainAndRun", ["CopyMainFiles"], shell.task("npm start", {cwd: DEBUG_OUTPUT_DIR}));

gulp.task("default", ["BuildDebug"]);