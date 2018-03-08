//
// Created by anlijiu on 18-1-24.
//

#ifndef CAN_META_HELPER_H
#define CAN_META_HELPER_H

#include <can/signal_meta.h>
#include <napi.h>
#include <can/message_meta.h>
#include "weapon/ammo/part_build_strategy.h"

namespace can {

std::pair<std::string, std::shared_ptr<SignalMeta>> unpack_signal_meta(Napi::Env env, Napi::Object signal_meta_obj);
std::pair<uint32_t , std::shared_ptr<MessageMeta>> unpack_message_meta(Napi::Env env, Napi::Object message_meta_obj);
std::pair<std::string, std::shared_ptr<PartBuildStrategy>> unpack_strategy(Napi::Env env, Napi::Object strategies_obj, std::map<std::string, std::shared_ptr<SignalMeta>> const&);

}
#endif //CAN_META_HELPER_H
