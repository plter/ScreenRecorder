const path = require('path');
const webpack = require('webpack');

module.exports = {
    entry: './main.js',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'bundle.js',
    },
    mode: "development",
    plugins: [
        new webpack.IgnorePlugin({
            checkResource(resource) {
                return ["path", "fs", "electron"].indexOf(resource) > -1;
            }
        })
    ]
};
