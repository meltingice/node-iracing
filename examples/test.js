var iRacing = require('../build/Release/iracing.node').iRacing;
var ir;

ir = new iRacing();
console.log(ir);
console.log(ir.waitForDataReady(60));