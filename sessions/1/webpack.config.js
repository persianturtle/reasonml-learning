const path = require('path')

module.exports = {
  mode: 'development',
  entry: './src/ReactIndex.bs.js',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.js'
  }
}
