var iRacing = require('../src').iRacing;
var ir;

console.log("Waiting for connection...");
console.log("Press Ctrl+C to quit");

iRacing.ready(function () {
  console.log(this.sessions.weekend.TrackDisplayName);

  var Throttle;
  this.onTick(function () {
    var _Throttle = this.getTelemetry('Throttle');
    if (_Throttle !== Throttle) {
      Throttle = _Throttle;
      console.log(Throttle);
    }
  });
});