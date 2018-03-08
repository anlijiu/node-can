#include <napi.h>
#include "agent.h"

using namespace Napi;



Napi::Object
init(Napi::Env env, Napi::Object exports)
{
    can::Agent::Init(env, exports);

    return exports;
}


NODE_API_MODULE(can, init)
