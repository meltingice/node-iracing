var iRacing = require('../dist').iRacing;
var ir;

ir = new iRacing();

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

while (true) {
  if (ir.waitForDataReady(60)) {
    console.log("Connection established!");
    console.log(ir.getDriver());

    break;
  }
}