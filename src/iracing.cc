#include <v8.h>
#include <node.h>

#include "../lib/yaml_parser.h"
#include "../lib/irsdk_defines.h"

using namespace v8;

char *data = NULL;
int dataLen = 0;

class IRacing: node::ObjectWrap {
private:
public:
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
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getTrack", GetTrack);
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getCarAndDriver", GetCarAndDriver);

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
    //IRacing* self = node::ObjectWrap::Unwrap<IRacing>(args.This());

    String::Utf8Value data(args[1]->ToString());
    bool result = irsdk_waitForDataReady(args[0]->Int32Value(), *data);
    return scope.Close(Boolean::New(result));
  }

  static Handle<Value> GetHeader(const Arguments& args) {
    HandleScope scope;
    //IRacing* inst = node::ObjectWrap::Unwrap<IRacing>(args.This());

    const irsdk_header *header = irsdk_getHeader();

    // if (!data || dataLen != header->bufLen) {
    //   if (data) {
    //     delete [] data;
    //   }

    //   char* data = new char[header->bufLen];
    // }

    Handle<Object> obj = Object::New();
    obj->Set(String::New("ver"), Int32::New(header->ver));
    obj->Set(String::New("status"), Int32::New(header->status));
    obj->Set(String::New("tickRate"), Int32::New(header->tickRate));

    return scope.Close(obj);
  }

  static Handle<Value> GetTrack(const Arguments& args) {
    HandleScope scope;

    const char* valueStr;
    int valueLen;

    if (parseYaml(irsdk_getSessionInfoStr(), "WeekendInfo:TrackName:", &valueStr, &valueLen)) {
      char* track = new char[valueLen];
      sprintf(track, "%.*s", valueLen, valueStr);

      return scope.Close(String::New(track));
    }
  }

  static Handle<Value> GetCarAndDriver(const Arguments& args) {
    HandleScope scope;

    const char* valStr;
    int valLen;
    char str[512];

    int carIdx = -1;

    Handle<Object> obj = Object::New();

    // Get the car ID
    if (parseYaml(irsdk_getSessionInfoStr(), "DriverInfo:DriverCarIdx:", &valStr, &valLen)) {
      carIdx = atoi(valStr);
      obj->Set(String::New("carIdx"), Int32::New(carIdx));
    }

    if (carIdx >= 0) {
      sprintf_s(str, 512, "DriverInfo:Drivers:CarIdx:{%d}UserName:", carIdx);
      if (parseYaml(irsdk_getSessionInfoStr(), str, &valStr, &valLen)) {
        obj->Set(String::New("driverName"), String::New(valStr, valLen));
      }

      sprintf_s(str, 512, "DriverInfo:Drivers:CarIdx:{%d}CarPath:", carIdx);
      if (parseYaml(irsdk_getSessionInfoStr(), str, &valStr, &valLen)) {
        obj->Set(String::New("driverCar"), String::New(valStr, valLen));
      }

      sprintf_s(str, 512, "DriverInfo:Drivers:CarIdx:{%d}CarNumber:", carIdx);
      if(parseYaml(irsdk_getSessionInfoStr(), str, &valStr, &valLen)) {
        obj->Set(String::New("driverCarNumber"), String::New(valStr, valLen));
      }
    }

    return scope.Close(obj);
  }
};

Persistent<FunctionTemplate> IRacing::pft;
extern "C" {
  static void init (Handle<Object> target) {
    IRacing::Init(target);
  }

  NODE_MODULE(iracing, init);
}