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
const MODE = "development";// development or production

function copyPackageJson() {
    return gulp.src("package.json").pipe(gulp.dest(OUTPUT_APP_DIR));
}

function compileMain() {
    js2wc.jsfile2wcfile(path.join(PROJECT_DIR, "main.js"), path.join(OUTPUT_OBJ_DIR, "main.cpp"));
    shelljs.exec(
        `docker run --rm -t -v ${OUTPUT_OBJ_DIR}:/Source -w /Source xtiqin/emsdk emcc \
        -std=c++11 --bind -Wall ${MODE === "production" ? "-O2" : ""} \
        -s ENVIRONMENT=node \
        main.cpp -o main.js`);
    return gulp.src([
        path.join(OUTPUT_OBJ_DIR, "main.js"),
        path.join(OUTPUT_OBJ_DIR, "main.wasm"),
    ]).pipe(gulp.dest(path.join(OUTPUT_APP_DIR)));
}

function npmInstall(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    let exitCode;
    if (!(exitCode = shelljs.exec("npm install"))) {
        console.error(`Failed to run 'npm install', exit code is ${exitCode}`);
    }
    cb();
}

function createBuildTarget(targetName, entryHtmlFile, entryJsFile, objDir, outputDir) {
    let entryJsFileNameWithoutExtension = path.basename(entryJsFile, ".js");

    function copyHtml() {
        return gulp.src(entryHtmlFile)
            .pipe(gulp.dest(outputDir));
    }

    function packJs() {
        return gulp.src(entryJsFile)
            .pipe(webpackStream({
                output: {
                    filename: `${entryJsFileNameWithoutExtension}.webpack.js`,
                },
                mode: MODE
            }))
            .pipe(gulp.dest(objDir));
    }

    function compilePackedJs() {
        js2wc.jsfile2wcfile(path.join(objDir, `${entryJsFileNameWithoutExtension}.webpack.js`), path.join(objDir, `${entryJsFileNameWithoutExtension}.cpp`));
        shelljs.exec(`
        docker run --rm -t -v ${objDir}:/Source -w /Source xtiqin/emsdk emcc \
        -std=c++11 --bind -Wall ${MODE === "production" ? "-O2" : ""} \
        -s ENVIRONMENT=node \
        ${entryJsFileNameWithoutExtension}.cpp -o ${entryJsFileNameWithoutExtension}.js`);
        return gulp.src([
            path.join(objDir, `${entryJsFileNameWithoutExtension}.js`),
            path.join(objDir, `${entryJsFileNameWithoutExtension}.wasm`)
        ]).pipe(gulp.dest(outputDir));
    }

    module.exports[targetName] = gulp.series(copyHtml, packJs, compilePackedJs);
}

createBuildTarget(
    "BuildRendererIndex",
    path.join(PROJECT_DIR, "src", "renderers", "index", "index.html"),
    path.join(PROJECT_DIR, "src", "renderers", "index", "IndexEntry.js"),
    path.join(OUTPUT_OBJ_DIR, "src", "renderers", "index"),
    path.join(OUTPUT_APP_DIR, "src", "renderers", "index")
);

createBuildTarget(
    "BuildRendererSettings",
    path.join(PROJECT_DIR, "src", "renderers", "settings", "settings.html"),
    path.join(PROJECT_DIR, "src", "renderers", "settings", "settings.js"),
    path.join(OUTPUT_OBJ_DIR, "src", "renderers", "settings"),
    path.join(OUTPUT_APP_DIR, "src", "renderers", "settings")
);


createBuildTarget(
    "BuildRendererVideoLibrary",
    path.join(PROJECT_DIR, "src", "renderers", "videolibrary", "VideoLibrary.html"),
    path.join(PROJECT_DIR, "src", "renderers", "videolibrary", "VideoLibrary.js"),
    path.join(OUTPUT_OBJ_DIR, "src", "renderers", "videolibrary"),
    path.join(OUTPUT_APP_DIR, "src", "renderers", "videolibrary")
);

function buildRendererAbout() {
    return gulp.src(path.join(PROJECT_DIR, "src", "renderers", "about", "**"))
        .pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "renderers", "about")));
}

function run(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    shelljs.exec("npm run start");
    cb();
}

function copyResFiles() {
    return gulp.src(path.join(PROJECT_DIR, "src", "res", "**")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "res")));
}

function copyLibFiles() {
    return gulp.src(path.join(PROJECT_DIR, "src", "lib", "**")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "lib")));
}

module.exports.build = gulp.series(
    copyPackageJson,
    compileMain,
    module.exports.BuildRendererIndex,
    buildRendererAbout,
    copyResFiles,
    copyLibFiles,
    module.exports.BuildRendererSettings,
    module.exports.BuildRendererVideoLibrary
);

module.exports.default = module.exports.build;
module.exports.BuildAndRun = gulp.series(module.exports.build, run);
module.exports.BuildAndInstallAndRun = gulp.series(module.exports.build, npmInstall, run);

