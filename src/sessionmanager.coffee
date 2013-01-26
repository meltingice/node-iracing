Session = require './session'

module.exports = class SessionManager
  iRacing = null

  @currentSessionId = 0
  @sessions = {}
  @current = null
  @weekend = null

  @load: (_iRacing, @data) ->
    iRacing = _iRacing
    @currentSessionId = @data.WeekendInfo.SessionID

    for session in @data.SessionInfo.Sessions
      @sessions[session.SessionNum] = new Session(iRacing, session)

    @current = @sessions[@currentSessionId]
    @weekend = @data.WeekendInfo

  @get: (id) -> @sessions[id]