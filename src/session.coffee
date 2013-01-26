Car = require './car'

module.exports = class Session
  iRacing = null
  
  constructor: (_iRacing, data) ->
    iRacing = _iRacing
    @id = data.SessionNum

    # We just preserve the original key names for
    # consistency.
    @[key] = val for own key, val of data

  results: ->
    results = []
    for position in @ResultsPositions
      position.Car = @iRacing.CarManager.get(position.CarIdx)
      results.push position

    results
