require('coffee-script');
var yaml = require('js-yaml');

// Load the C++ bindings
var iRacing = require('../build/Release/iracing.node').iRacing;

// Time between data ticks in ms
iRacing.TIMEOUT = 60;

// Our various modules
iRacing.SessionManager = require("./sessionmanager");
iRacing.prototype.sessions = iRacing.SessionManager;

iRacing.CarManager = require("./carmanager");
iRacing.prototype.cars = iRacing.CarManager;

// Called when we have a connection to initialize the data
// for all the various modules. This should never be called
// by the user, so we don't expose it.
var initialize = function (instance) {
  var sessionData = yaml.load(instance.getSessionYAML());
  iRacing.SessionManager.load(iRacing, sessionData);
  iRacing.CarManager.load(iRacing, sessionData);
};

// Execute a callback once we are connected
iRacing.ready = function (cb) {
  var ir = new iRacing();

  setTimeout(function () {
    while (true) {
      if (this.waitForDataReady(iRacing.TIMEOUT)) {
        initialize(ir);
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

// Export our iRacing object
exports.iRacing = iRacing;