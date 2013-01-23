// Right now we do YAML parsing in Javascript since it's
// a serious pain in C++. This should move back to native
// C++ parsing in the future for speed.
var yaml = require('js-yaml');

// Load the C++ bindings
var iRacing = require('../build/Release/iracing.node').iRacing;

// Helper methods

iRacing.prototype.getSession = function () {
  return yaml.load(this.getSessionYAML());
};

iRacing.prototype.getDriver = function () {
  var id = this.getCarIdx();
  return this.getSession().DriverInfo.Drivers.filter(function (driver) {
    return driver.CarIdx === id;
  })[0];
};

// Export our iRacing object
exports.iRacing = iRacing;