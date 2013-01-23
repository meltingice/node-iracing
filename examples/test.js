var iRacing = require('../build/Release/iracing.node').iRacing;
var ir;

ir = new iRacing();

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

while (true) {
  if (ir.waitForDataReady(60)) {
    console.log("Connection established!");
    console.log(ir.getHeader());
    console.log(ir.getTrack());
    console.log(ir.getCarAndDriver());

    break;
  }
}