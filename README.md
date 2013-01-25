# iRacing Bindings for Node

This is a native C++ library that binds the iRacing SDK to NodeJS so that you can access it from Javascript.

## Example Usage

``` javascript
var iRacing = require('iracing').iRacing;

console.log("Waiting for connection...");

iRacing.ready(function () {
  console.log("Connected!");
  console.log(this.getDriver());

  this.onTick(function () {
    console.log(this.getTelemetry('Throttle'));
  });
});
```