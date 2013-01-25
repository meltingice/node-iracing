// Right now we do YAML parsing in Javascript since it's
// a serious pain in C++. This should move back to native
// C++ parsing in the future for speed.
var yaml = require('js-yaml');

// Load the C++ bindings
var iRacing = require('../build/Release/iracing.node').iRacing;

// Time between data ticks in ms
iRacing.TIMEOUT = 60;

// Execute a callback once we are connected
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

// Execute a callback for every data tick
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

iRacing.prototype.getCurrentSession = function () {
  var sessionId = this.getSession().WeekendInfo.SessionID;
  return this.getSessionById(sessionId);
};

iRacing.prototype.getSessionById = function (id) {
  var sessions = this.getSession().SessionInfo.Sessions;

  for (var i = 0, _len = sessions.length; i < _len; i++) {
    if (sessions[i].SessionNum === id) {
      return sessions[i];
    }
  }
};

// Get the driver's car ID
iRacing.prototype.getCarId = function () {
  return this.getSession().DriverInfo.DriverCarIdx;
};

// Get the current driver
iRacing.prototype.getDriver = function () {
  return this.getDriverById(this.getCarId());
};

iRacing.prototype.getDriverById = function (id) {
  var driver;
  var drivers = this.getSession().DriverInfo.Drivers;
  
  for (var i = 0, _len = drivers.length; i < _len; i++) {
    if (drivers[i].CarIdx === id) {
      return drivers[i];
    }
  }
  
  return null;
};

iRacing.prototype.getSplits = function () {

};

// Export our iRacing object
exports.iRacing = iRacing;