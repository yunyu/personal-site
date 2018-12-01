const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');

const outputDir = path.join(__dirname, 'build/');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
    entry: './src/Index.bs.js',
    mode: isProd ? 'production' : 'development',
    output: {
        path: outputDir,
        publicPath: '/',
        filename: 'assets/build.[hash].js'
    },
    plugins: [
        new HtmlWebpackPlugin({
            favicon: 'favicon.png',
            template: 'src/index.html',
            inject: true
        }),
        new MiniCssExtractPlugin({
            filename: 'assets/[name].[hash].css',
            chunkFilename: 'assets/[id].[hash].css'
        })
    ],
    module: {
        rules: [
            {
                test: /\.css$/,
                use: [
                    isProd ? MiniCssExtractPlugin.loader : 'style-loader',
                    'css-loader',
                    'postcss-loader'
                ]
            }
        ]
    },
    devServer: {
        compress: true,
        contentBase: outputDir,
        port: process.env.PORT || 8000,
        historyApiFallback: true
    }
};
