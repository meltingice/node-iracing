#include <v8.h>
#include <node.h>

#include "../lib/irsdk_defines.h"

using namespace v8;
using namespace std;

class IRacing: node::ObjectWrap {
private:
public:
  static char* data;
  static int dataLen;

  IRacing() {}
  ~IRacing() {}

  static Persistent<FunctionTemplate> pft;

  static void Init(Handle<Object> target) {
    HandleScope scope;

    Local<FunctionTemplate> lft = FunctionTemplate::New(New);
    
    IRacing::pft = Persistent<FunctionTemplate>::New(lft);
    IRacing::pft->InstanceTemplate()->SetInternalFieldCount(1);
    IRacing::pft->SetClassName(String::NewSymbol("IRacing"));

    // Methods
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "waitForDataReady", WaitForDataReady);
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getHeader", GetHeader);
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getSessionYAML", GetSessionYAML);
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getTelemetry", GetTelemetry);

    target->Set(String::NewSymbol("iRacing"), IRacing::pft->GetFunction());
  }

  static Handle<Value> New(const Arguments& args) {
    HandleScope scope;

    IRacing* self = new IRacing();
    self->Wrap(args.This());
    
    return scope.Close(args.This());
  }

  static Handle<Value> WaitForDataReady(const Arguments& args) {
    HandleScope scope;

    bool result = irsdk_waitForDataReady(args[0]->Int32Value(), data);

    if (result) {
      const irsdk_header *header = irsdk_getHeader();

      if (!data || dataLen != header->bufLen) {
        if (data)
          delete [] data;

        dataLen = header->bufLen;
        data = new char[dataLen];

        irsdk_waitForDataReady(args[0]->Int32Value(), data);  
      }
    }

    return scope.Close(Boolean::New(result));
  }

  static Handle<Value> GetHeader(const Arguments& args) {
    HandleScope scope;
    //IRacing* inst = node::ObjectWrap::Unwrap<IRacing>(args.This());

    const irsdk_header *header = irsdk_getHeader();

    // if (header && (!data || dataLen != header->bufLen)) {
    //   if (data) {
    //     delete [] data;
    //   }

    //   dataLen = header->bufLen;
    //   data = new char[dataLen];
    // }

    Handle<Object> obj = Object::New();
    obj->Set(String::New("ver"), Int32::New(header->ver));
    obj->Set(String::New("status"), Int32::New(header->status));
    obj->Set(String::New("tickRate"), Int32::New(header->tickRate));

    return scope.Close(obj);
  }

  static Handle<Value> GetSessionYAML(const Arguments& args) {
    HandleScope scope;

    return scope.Close(String::New(irsdk_getSessionInfoStr()));
  }

  static Handle<Value> GetTelemetry(const Arguments& args) {
    HandleScope scope;

    String::Utf8Value propName(args[0]->ToString());
    int offset = irsdk_varNameToOffset(*propName);

    if ( // First, the ints
      !strcmp(*propName, "CamCameraNumber") ||
      !strcmp(*propName, "CamCarIdx") ||
      !strcmp(*propName, "CamGroupNumber") ||
      !strcmp(*propName, "RaceLaps") ||
      !strcmp(*propName, "ReplayFrameNum") ||
      !strcmp(*propName, "ReplayFrameNumEnd") ||
      !strcmp(*propName, "ReplayPlaySpeed") ||
      !strcmp(*propName, "ReplaySessionNum") ||
      !strcmp(*propName, "Gear") ||
      !strcmp(*propName, "Lap") ||
      !strcmp(*propName, "LapBestLap") ||
      !strcmp(*propName, "SessionLapsRemain") ||
      !strcmp(*propName, "SessionNum") ||
      !strcmp(*propName, "SessionState") ||
      !strcmp(*propName, "SessionUniqueID")
      ) {

      return scope.Close(Number::New(*((int *)(data + offset))));

    } else if (
      !strcmp(*propName, "Brake") ||
      !strcmp(*propName, "Clutch") ||
      !strcmp(*propName, "FuelLevel") ||
      !strcmp(*propName, "FuelLevelPct") ||
      !strcmp(*propName, "FuelPress") ||
      !strcmp(*propName, "LapBestLapTime") ||
      !strcmp(*propName, "LapCurrentLapTime") ||
      !strcmp(*propName, "LapDeltaToBestLap") ||
      !strcmp(*propName, "LapDeltaToBestLap_DD") ||
      !strcmp(*propName, "LapDeltaToOptimalLap") ||
      !strcmp(*propName, "LapDeltaToOptimalLap_DD") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap_DD") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap_DD") ||
      !strcmp(*propName, "LapDist") ||
      !strcmp(*propName, "LapDistPct") ||
      !strcmp(*propName, "LapLastLapTime") ||
      !strcmp(*propName, "LatAccel") ||
      !strcmp(*propName, "LFshockDefl") ||
      !strcmp(*propName, "LongAccel") ||
      !strcmp(*propName, "LRshockDefl") ||
      !strcmp(*propName, "ManifoldPress") ||
      !strcmp(*propName, "OilLevel") ||
      !strcmp(*propName, "OilPress") ||
      !strcmp(*propName, "OilTemp") ||
      !strcmp(*propName, "Pitch") ||
      !strcmp(*propName, "PitchRate") ||
      !strcmp(*propName, "PitOptRepairLeft") ||
      !strcmp(*propName, "PitRepairLeft") ||
      !strcmp(*propName, "RFshockDefl") ||
      !strcmp(*propName, "Roll") ||
      !strcmp(*propName, "RollRate") ||
      !strcmp(*propName, "RPM") ||
      !strcmp(*propName, "RRshockDefl") ||
      !strcmp(*propName, "ShiftGrindRPM") ||
      !strcmp(*propName, "ShiftIndicatorPct") ||
      !strcmp(*propName, "ShiftPowerPct") ||
      !strcmp(*propName, "Speed") ||
      !strcmp(*propName, "SteeringWheelAngle") ||
      !strcmp(*propName, "SteeringWheelPctTorque") ||
      !strcmp(*propName, "SteeringWheelTorque") ||
      !strcmp(*propName, "Throttle") ||
      !strcmp(*propName, "VelocityX") ||
      !strcmp(*propName, "VelocityY") ||
      !strcmp(*propName, "VelocityZ") ||
      !strcmp(*propName, "VertAccel") ||
      !strcmp(*propName, "Voltage") ||
      !strcmp(*propName, "WaterLevel") ||
      !strcmp(*propName, "WaterTemp") ||
      !strcmp(*propName, "Yaw") ||
      !strcmp(*propName, "YawRate") ||
      !strcmp(*propName, "Alt") ||
      !strcmp(*propName, "LFpressure") ||
      !strcmp(*propName, "LFrideHeight") ||
      !strcmp(*propName, "LFspeed") ||
      !strcmp(*propName, "LFtempL") ||
      !strcmp(*propName, "LFtempM") ||
      !strcmp(*propName, "LFtempR") ||
      !strcmp(*propName, "LRpressure") ||
      !strcmp(*propName, "LRrideHeight") ||
      !strcmp(*propName, "LRspeed") ||
      !strcmp(*propName, "LRtempL") ||
      !strcmp(*propName, "LRtempM") ||
      !strcmp(*propName, "LRtempR") ||
      !strcmp(*propName, "RFpressure") ||
      !strcmp(*propName, "RFrideHeight") ||
      !strcmp(*propName, "RFspeed") ||
      !strcmp(*propName, "RFtempL") ||
      !strcmp(*propName, "RFtempM") ||
      !strcmp(*propName, "RFtempR") ||
      !strcmp(*propName, "RRpressure") ||
      !strcmp(*propName, "RRrideHeight") ||
      !strcmp(*propName, "RRspeed") ||
      !strcmp(*propName, "RRtempL") ||
      !strcmp(*propName, "RRtempM") ||
      !strcmp(*propName, "RRtempR")
      ) {

      return scope.Close(Number::New(*((float *)(data + offset))));

    } else if (
      !strcmp(*propName, "ReplaySessionTime") ||
      !strcmp(*propName, "SessionTime") ||
      !strcmp(*propName, "SessionTimeRemain") ||
      !strcmp(*propName, "Lat") ||
      !strcmp(*propName, "Lon")
      ) {

      return scope.Close(Number::New(*((double *)(data + offset))));

    } else if (
      !strcmp(*propName, "IsInGarage") ||
      !strcmp(*propName, "IsReplayPlaying") ||
      !strcmp(*propName, "ReplayPlaySlowMotion") ||
      !strcmp(*propName, "DriverMarker") ||
      !strcmp(*propName, "IsOnTrack") ||
      !strcmp(*propName, "LapDeltaToOptimalLap_OK") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap_OK") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap_OK") ||
      !strcmp(*propName, "OnPitRoad")
      ) {

      return scope.Close(Boolean::New(*((bool *)(data + offset))));

    }
  }
};

Persistent<FunctionTemplate> IRacing::pft;

char* IRacing::data = NULL;
int IRacing::dataLen = 0;

extern "C" {
  static void init (Handle<Object> target) {
    IRacing::Init(target);
  }

  NODE_MODULE(iracing, init);
}