const gulp = require("gulp");
const path = require("path");
const webpackStream = require('webpack-stream');
const webpack = require('webpack');
const js2wc = require('js2wc');
const shelljs = require("shelljs");

const PROJECT_DIR = __dirname;
const OUTPUT_DIR = path.join(PROJECT_DIR, "dist");
const OUTPUT_OBJ_DIR = path.join(OUTPUT_DIR, "obj");
const OUTPUT_APP_DIR = path.join(OUTPUT_DIR, "app");
const MODE = "dev";//or production

function copyPackageJson(cb) {
    gulp.src("package.json").pipe(gulp.dest(OUTPUT_APP_DIR));
    cb();
}

function compileMain(cb) {
    js2wc.jsfile2wcfile(path.join(PROJECT_DIR, "main.js"), path.join(OUTPUT_OBJ_DIR, "main.cpp"));
    shelljs.exec(
        `docker run --rm -t -v ${OUTPUT_OBJ_DIR}:/Source -w /Source xtiqin/emsdk emcc \
        -std=c++11 --bind -Wall ${MODE === "production" ? "-O2" : ""} \
        -s ENVIRONMENT=node \
        main.cpp -o main.js`);
    gulp.src([
        path.join(OUTPUT_OBJ_DIR, "main.js"),
        path.join(OUTPUT_OBJ_DIR, "main.wasm"),
    ]).pipe(gulp.dest(path.join(OUTPUT_APP_DIR)));
    cb();
}

function npmInstall(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    let exitCode;
    if (!(exitCode = shelljs.exec("npm install"))) {
        console.error(`Failed to run 'npm install', exit code is ${exitCode}`);
    }
    cb();
}

function copyRendererIndex(cb) {
    gulp.src(path.join(PROJECT_DIR, "src", "renderers", "index", "index.html"))
        .pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "renderers", "index")));
    cb();
}

function run(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    shelljs.exec("npm run start");
    cb();
}

function copyResFiles(cb) {
    gulp.src(path.join(PROJECT_DIR, "src", "res", "**")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "res")));
    cb();
}

module.exports.build = gulp.series(copyPackageJson, compileMain, copyRendererIndex, copyResFiles);
module.exports.default = module.exports.build;
module.exports.BuildAndRun = gulp.series(module.exports.build, run);

