#include <nan.h>
#include "vieworks.h"

void InitAll(v8::Local<v8::Object> exports) {
  vwCam::Init(exports);
}

NODE_MODULE(vieworks, InitAll)
