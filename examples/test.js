var iRacing = require('../dist').iRacing;
var ir;

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

iRacing.ready(function () {
  console.log(this.getDriver());

  var Brake;
  this.onTick(function () {
    var _Brake = this.getTelemetry('Brake');
    if (_Brake !== Brake) {
      Brake = _Brake;
      console.log("Brake is now " + Brake);
    }
  });
});