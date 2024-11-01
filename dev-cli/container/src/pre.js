/* eslint-disable */

console.log('======= VIC ====== RUNNING PRE.JS')

require("didkit_loader.js");

Module.locateFile = (url) => {
  //console.log('file', url)
  return url
}

// Module.getBinaryPromise = (f) => {
//   conosle.log(f)
//   console.log('hyperbeam', Module['hyperbeam'])
//   return Promise.resolve('foo')
// }