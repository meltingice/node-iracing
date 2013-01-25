{iRacing} = require '../dist'

console.log "Waiting for connection..."

iRacing.ready ->
  console.log "Connected!"

  data = {}
  @onTick ->
    for item in ["Gear", "Throttle"]
      newVal = @getTelemetry item
      if newVal isnt data[item]
        console.log item, newVal
        data[item] = newVal