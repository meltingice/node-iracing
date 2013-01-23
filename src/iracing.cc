#include <v8.h>
#include <node.h>

#include "../lib/yaml_parser.h"
#include "../lib/irsdk_defines.h"

using namespace v8;

class IRacing: node::ObjectWrap {
private:
public:
  static char* dataStr;
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
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getCarIdx", GetCarIdx);
    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "getTelemetryData", GetTelemetryData);

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

    bool result = irsdk_waitForDataReady(args[0]->Int32Value(), dataStr);
    return scope.Close(Boolean::New(result));
  }

  static Handle<Value> GetHeader(const Arguments& args) {
    HandleScope scope;
    //IRacing* inst = node::ObjectWrap::Unwrap<IRacing>(args.This());

    const irsdk_header *header = irsdk_getHeader();

    if (header && (!dataStr || dataLen != header->bufLen)) {
      if (dataStr) {
        delete [] dataStr;
      }

      dataLen = header->bufLen;
      dataStr = new char[dataLen];
    }

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

  static Handle<Value> GetCarIdx(const Arguments& args) {
    HandleScope scope;

    const char* valStr;
    int valLen;

    int carIdx = -1;

    // Get the car ID
    if (parseYaml(irsdk_getSessionInfoStr(), "DriverInfo:DriverCarIdx:", &valStr, &valLen)) {
      carIdx = atoi(valStr);
      return scope.Close(Int32::New(carIdx));
    }
  }

  static Handle<Value> GetTelemetryData(const Arguments& args) {
    HandleScope scope;

    String::Utf8Value propName(args[0]->ToString());
    int offset = irsdk_varNameToOffset(*propName);
    printf("%d, %d\n", offset, dataLen);

    Local<Number> val;
    if (strcmp(*propName, "Gear") == 0) {
      val = Number::New(*((int *)(dataStr + offset)));
    } else if (strcmp(*propName, "OilLevel") == 0) {
      val = Number::New(*((float *)(dataStr + offset)));
    }

    return scope.Close(val);
  }
};

Persistent<FunctionTemplate> IRacing::pft;
char* IRacing::dataStr = NULL;
int IRacing::dataLen = 0;

extern "C" {
  static void init (Handle<Object> target) {
    IRacing::Init(target);
  }

  NODE_MODULE(iracing, init);
}