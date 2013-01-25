var iRacing = require('../dist').iRacing;
var ir;

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

iRacing.ready(function () {
  console.log(this.getDriver());
});