module.exports = class Car
  iRacing = null

  constructor: (_iRacing, data) ->
    _iRacing = iRacing

    @isDriver = false
    @[key] = val for own key, val of data

  setDriverInfo: (data) ->
    delete data.DriverCarIdx
    delete data.Drivers

    @isDriver = true
    @[key] = val for own key, val of data