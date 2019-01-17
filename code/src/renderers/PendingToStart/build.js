const gulp = require("gulp");
const path = require("path");


gulp.task("CopyRendererPendingToStartHTML", function () {
    gulp.src(path.join(__dirname, "PendingToStart.html")).pipe(gulp.dest(RENDERERS_DEBUG_OUTPUT_DIR));
});