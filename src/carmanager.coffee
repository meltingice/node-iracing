Car = require './car'

module.exports = class CarManager
  iRacing = null

  @cars = {}
  @driverCarId = 0
  @driverCar = null

  @load: (_iRacing, data) ->
    iRacing = _iRacing
    @driverCarId = data.DriverInfo.DriverCarIdx

    for car in data.DriverInfo.Drivers
      @cars[car.CarIdx] = new Car(iRacing, car)

    @driverCar = @cars[@driverCarId]
    @driverCar.setDriverInfo(data.DriverInfo)

  @get: (id) -> @cars[id]