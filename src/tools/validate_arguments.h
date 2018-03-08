//
// Created by anlijiu on 1/23/18.
//

#ifndef CAN_VALIDATEARGUMENTS_H
#define CAN_VALIDATEARGUMENTS_H

#include <napi.h>
#include <sstream>
#include <string>
#include <vector>

using namespace Napi;
using namespace std;

namespace can {

static void
AssertFunctionArgs(const CallbackInfo& info,
                   int expectedCount,
                   const vector<napi_valuetype>& expectedTypes)
{
  stringstream eMsg;
  if (info.Length() < static_cast<size_t>(expectedCount)) {
    eMsg << "Expected " << expectedCount << " but received " << info.Length()
         << endl;
    throw Napi::Error::New(info.Env(), eMsg.str());
  }
  for (size_t i = 0; i < info.Length(); ++i) {
    if (expectedTypes.size() - 1 == i)
      break;
    napi_valuetype expected = expectedTypes[i];
    napi_valuetype actual = info[i].Type();
    if (actual != expected) {
      eMsg << "Expected parameter[" << i << "] to be of type: " << expected
           << " but received: " << actual << endl;
      throw Napi::Error::New(info.Env(), eMsg.str());
    }
  }
}

} // namespace can

#endif // CAN_VALIDATEARGUMENTS_H
