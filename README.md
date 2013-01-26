# iRacing Bindings for Node

A native C++ library that binds the iRacing SDK to NodeJS so that you can access it from Javascript.

## Example Usage

``` javascript
var iRacing = require('iracing').iRacing;

console.log("Waiting for connection...");

// Wait for the game connection
iRacing.ready(function () {
  console.log("Connected!");
  console.log(this.sessions.weekend.TrackDisplayName);

  // Execute code on every simulation tick
  this.onTick(function () {
    console.log(this.getTelemetry('Throttle'));
  });
});
```