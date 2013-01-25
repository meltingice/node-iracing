var iRacing = require('../dist').iRacing;
var ir;

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

iRacing.ready(function () {
  console.log(this.getDriver());

  var Gear;
  this.onTick(function () {
    var _Gear = this.getTelemetry('Gear');
    if (_Gear !== Gear) {
      Gear = _Gear;
      console.log("Gear is now " + Gear);
    }
  });
});