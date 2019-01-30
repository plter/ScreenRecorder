const gulp = require("gulp");
const path = require("path");
const webpackStream = require('webpack-stream');
const webpack = require('webpack');
const js2wc = require('js2wc');
const shelljs = require("shelljs");
const os = require("os");

const PROJECT_DIR = __dirname;
const OUTPUT_DIR = path.join(PROJECT_DIR, "dist");
const OUTPUT_OBJ_DIR = path.join(OUTPUT_DIR, "obj");
const OUTPUT_APP_DIR = path.join(OUTPUT_DIR, "app");
let MODE = "development";// development or production
let COMPILE_LEVEL = "";

function copyPackageJson() {
    return gulp.src("package.json").pipe(gulp.dest(OUTPUT_APP_DIR));
}

function compileMain(cb) {
    js2wc.jsfile2wcfile(path.join(PROJECT_DIR, "main.js"), path.join(OUTPUT_OBJ_DIR, "main.cpp"));
    let exitCode = shelljs.exec(
        `docker run --rm -t -v ${OUTPUT_OBJ_DIR}:/Source -w /Source xtiqin/emsdk emcc \
        -std=c++11 --bind -Wall ${COMPILE_LEVEL} \
        -s ENVIRONMENT=node \
        main.cpp -o main.js`).code;
    if (exitCode) {
        cb(new Error(`Compile main.cpp failed with exit code ${exitCode}`));
        return null;
    } else {
        return gulp.src([
            path.join(OUTPUT_OBJ_DIR, "main.js"),
            path.join(OUTPUT_OBJ_DIR, "main.wasm"),
        ]).pipe(gulp.dest(path.join(OUTPUT_APP_DIR)));
    }
}

function npmInstall(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    let exitCode;
    if (!(exitCode = shelljs.exec("npm install"))) {
        cb(new Error(`Failed to run 'npm install', exit code is ${exitCode}`));
    } else {
        cb();
    }
}

function createBuildTarget(targetName, entryHtmlFile, entryJsFile, objDir, outputDir) {
    let entryJsFileNameWithoutExtension = path.basename(entryJsFile, ".js");

    function copyHtml(cb) {
        if (entryHtmlFile) {
            return gulp.src(entryHtmlFile)
                .pipe(gulp.dest(outputDir));
        } else {
            cb();
        }
    }

    function packJs() {
        return gulp.src(entryJsFile)
            .pipe(webpackStream({
                output: {
                    filename: `${entryJsFileNameWithoutExtension}.webpack.js`,
                },
                mode: MODE,
                target: "node"
            }))
            .pipe(gulp.dest(objDir));
    }

    function compilePackedJs(cb) {
        js2wc.jsfile2wcfile(path.join(objDir, `${entryJsFileNameWithoutExtension}.webpack.js`), path.join(objDir, `${entryJsFileNameWithoutExtension}.cpp`));
        let exitCode = shelljs.exec(`
        docker run --rm -t -v ${objDir}:/Source -w /Source xtiqin/emsdk emcc \
        -std=c++11 --bind -Wall ${COMPILE_LEVEL} \
        -s ENVIRONMENT=node \
        ${entryJsFileNameWithoutExtension}.cpp -o ${entryJsFileNameWithoutExtension}.js`).code;
        if (exitCode) {
            cb(new Error(`Compile ${entryJsFileNameWithoutExtension}.cpp failed with exit code ${exitCode}`));
            return null;
        } else {
            return gulp.src([
                path.join(objDir, `${entryJsFileNameWithoutExtension}.js`),
                path.join(objDir, `${entryJsFileNameWithoutExtension}.wasm`)
            ]).pipe(gulp.dest(outputDir));
        }
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

createBuildTarget(
    "BuildRendererScreenChooser",
    path.join(PROJECT_DIR, "src", "renderers", "ScreenChooser", "ScreenChooser.html"),
    path.join(PROJECT_DIR, "src", "renderers", "ScreenChooser", "ScreenChooser.js"),
    path.join(OUTPUT_OBJ_DIR, "src", "renderers", "ScreenChooser"),
    path.join(OUTPUT_APP_DIR, "src", "renderers", "ScreenChooser")
);

createBuildTarget(
    "BuildRendererSrvReaderWorker",
    null,
    path.join(PROJECT_DIR, "src", "renderers", "commons", "workers", "SrvReader", "SrvReaderWorker.js"),
    path.join(OUTPUT_OBJ_DIR, "src", "renderers", "commons", "workers", "SrvReader"),
    path.join(OUTPUT_APP_DIR, "src", "renderers", "commons", "workers", "SrvReader")
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

function buildRendererPendingToStart() {
    return gulp.src(path.join(PROJECT_DIR, "src", "renderers", "PendingToStart", "PendingToStart.html"))
        .pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "renderers", "PendingToStart")));
}


function copyFfmpeg(cb) {
    let type = os.type();
    let arch = os.arch();
    switch (type) {
        case "Darwin":
            switch (arch) {
                case "x64":
                    gulp.src(path.join(PROJECT_DIR, "src", "bin", "ffmpeg_osx64")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "bin")));
                    break;
                default:
                    console.error(`Unsupported arch for platform ${type}`);
                    break;
            }
            break;
        case "Windows_NT":
            switch (arch) {
                case "x64":
                    gulp.src(path.join(PROJECT_DIR, "src", "bin", "ffmpeg_win64.exe")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "bin")));
                    break;
                case "x32":
                case "ia32":
                    gulp.src(path.join(PROJECT_DIR, "src", "bin", "ffmpeg_win32.exe")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "bin")));
                    break;
                default:
                    console.error(`Unsupported arch for platform ${type}`);
                    break;
            }
            break;
        case "Linux":
            switch (arch) {
                case "x64":
                    gulp.src(path.join(PROJECT_DIR, "src", "bin", "ffmpeg_linux64.exe")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "bin")));
                    break;
                case "x32":
                case "ia32":
                    gulp.src(path.join(PROJECT_DIR, "src", "bin", "ffmpeg_linux32.exe")).pipe(gulp.dest(path.join(OUTPUT_APP_DIR, "src", "bin")));
                    break;
                default:
                    console.error(`Unsupported arch for platform ${type}`);
                    break;
            }
            break;
        default:
            console.error("Unsupported platform");
            break;
    }
    cb();
}

function buildInstaller(cb) {
    shelljs.cd(OUTPUT_APP_DIR);
    shelljs.exec(`npm run packInstaller`);
    cb();
}

function setModeToProduction(cb) {
    MODE = "production";
    cb();
}

function setCompileLevelToO2(cb) {
    COMPILE_LEVEL = "-O2";
    cb();
}


module.exports.build = gulp.series(
    copyPackageJson,
    compileMain,
    module.exports.BuildRendererIndex,
    buildRendererAbout,
    copyResFiles,
    copyLibFiles,
    module.exports.BuildRendererSettings,
    module.exports.BuildRendererVideoLibrary,
    buildRendererPendingToStart,
    copyFfmpeg,
    module.exports.BuildRendererSrvReaderWorker
);

module.exports.default = module.exports.build;
module.exports.BuildAndRun = gulp.series(module.exports.build, run);

module.exports.BuildAndInstallAndRun = gulp.series(module.exports.build, npmInstall, run);

/**
 * 将工程打包成安装包
 */
module.exports.production = gulp.series(setCompileLevelToO2, setModeToProduction, module.exports.default, npmInstall, buildInstaller);

