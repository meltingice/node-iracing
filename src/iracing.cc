#include <v8.h>
#include <node.h>

#include "../lib/yaml_parser.h"
#include "../lib/irsdk_defines.h"

using namespace v8;

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

    NODE_SET_PROTOTYPE_METHOD(IRacing::pft, "waitForDataReady", WaitForDataReady);

    target->Set(String::NewSymbol("iRacing"), IRacing::pft->GetFunction());
  }

  static Handle<Value> New(const Arguments& args) {
    HandleScope scope;

    IRacing* ir = new IRacing();
    ir->Wrap(args.This());
    
    return args.This();
  }

  static Handle<Value> WaitForDataReady(const Arguments& args) {
    HandleScope scope;
    IRacing* inst = node::ObjectWrap::Unwrap<IRacing>(args.This());

    String::Utf8Value data(args[1]->ToString());
    bool result = irsdk_waitForDataReady(args[0]->Int32Value(), *data);
    return Boolean::New(result);
  }

};

Persistent<FunctionTemplate> IRacing::pft;
extern "C" {
  static void init (Handle<Object> target) {
    IRacing::Init(target);
  }

  NODE_MODULE(iracing, init);
}