module.exports = class Car
  iRacing = null

  constructor: (_iRacing, data) ->
    _iRacing = iRacing
    @[key] = val for own key, val of data