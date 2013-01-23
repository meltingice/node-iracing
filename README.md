# iRacing Bindings for Node

This is a native C++ library that binds the iRacing SDK to NodeJS so that you can access it from Javascript.

**This project is new and currently under heavy development. Currently you can only read the session data.**

## Example Usage

``` javascript
var iRacing = require('iracing').iRacing;
var ir = new iRacing();

console.log("Waiting for connection...");

var cd;
while (true) {
  if (ir.waitForDataReady(60)) {
    console.log("Connection established");
    console.log(ir.getDriver());
    break;
  }
}
```