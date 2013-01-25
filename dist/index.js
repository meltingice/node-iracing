// Right now we do YAML parsing in Javascript since it's
// a serious pain in C++. This should move back to native
// C++ parsing in the future for speed.
var yaml = require('js-yaml');

// Load the C++ bindings
var iRacing = require('../build/Release/iracing.node').iRacing;
iRacing.TIMEOUT = 60;

iRacing.ready = function (cb) {
  var ir = new iRacing();

  setTimeout(function () {
    while (true) {
      if (this.waitForDataReady(iRacing.TIMEOUT)) {
        return cb.call(this);
      }
    }
  }.bind(ir), 0);
};

iRacing.prototype.onTick = function (cb) {
  setTimeout(function () {
    var result = true;

    while(true) {
      if (this.waitForDataReady(iRacing.TIMEOUT)) {
        result = cb.call(this);
        if (result === false) return;
      }
    }
  }.bind(this), 0);
};

// Since YAML parsing can be an expensive overhead, we cache
// the parsed result. TODO: we need to detect when the session
// data has changed so we can re-parse it.
iRacing.prototype.getSession = function () {
  if (this.sessionData) return this.sessionData;
  this.sessionData = yaml.load(this.getSessionYAML());

  return this.sessionData;
};

iRacing.prototype.getDriver = function () {
  var id = this.getCarIdx();
  return this.getSession().DriverInfo.Drivers.filter(function (driver) {
    return driver.CarIdx === id;
  })[0];
};

// Export our iRacing object
exports.iRacing = iRacing;