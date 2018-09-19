//
// Created by anlijiu on 18-1-24.
//

#include "meta_helper.h"
#include <iostream>
#include <sstream>

namespace can {



std::pair<std::string, std::shared_ptr<SignalMeta>>
unpack_signal_meta(Napi::Env env, Napi::Object signal_meta_obj) {
  std::string name = signal_meta_obj.Get("name").As<Napi::String>().Utf8Value();
  uint32_t start = signal_meta_obj.Get("start_bit").As<Napi::Number>().Uint32Value();
  uint32_t length = signal_meta_obj.Get("length").As<Napi::Number>().Uint32Value();
  double scaling = signal_meta_obj.Get("scaling").As<Napi::Number>().DoubleValue();
  double offset = signal_meta_obj.Get("offset").As<Napi::Number>().DoubleValue();
  double minimum = signal_meta_obj.Get("minimum").As<Napi::Number>().DoubleValue();
  double maximum = signal_meta_obj.Get("maximum").As<Napi::Number>().DoubleValue();
  bool is_signed = signal_meta_obj.Get("is_signed").As<Napi::Number>().Uint32Value() > 0;
  std::string units = "";

  return std::make_pair(name, std::make_shared<SignalMeta>(SignalMeta{
          name,
          start,
          length,
          ByteOrder::kBigEndian,
          scaling,
          offset,
          minimum,
          maximum,
          is_signed,
          units
  }));;
}

std::pair<uint32_t, std::shared_ptr<MessageMeta>>
unpack_message_meta(Napi::Env env, Napi::Object message_meta_obj) {
  std::string name = message_meta_obj.Get("name").As<Napi::String>().Utf8Value();
  uint32_t id = static_cast<uint32_t>(std::stoi(message_meta_obj.Get("id").As<Napi::String>().Utf8Value()));
  uint8_t dlc = static_cast<uint8_t>(message_meta_obj.Get("length").As<Napi::Number>().Uint32Value());
  auto msg_tp = std::make_shared<MessageMeta>(MessageMeta{
          name,
          id,
          dlc,
          std::vector<std::string>()
  });
  Napi::Value signalValue = message_meta_obj.Get("signals");
  if(signalValue.IsArray()) {
    Napi::Array signalArr = signalValue.As<Napi::Array>();
    for (unsigned int i = 0; i < signalArr.Length(); ++i) {
      std::string signal_name = signalArr.Get(i).As<Napi::String>().Utf8Value();
      msg_tp->signal_names.push_back(signal_name);
    }
  } 
  // else if(signalValue.IsObject()){
  //   Napi::Object signal_meta_obj = Napi::Object(env, signalValue.ToObject());
  //   msg_tp->signal_metas.push_back(unpack_signal_meta(env, signal_meta_obj));
  // }


  return std::make_pair(id, msg_tp);
}

std::pair<std::string, std::shared_ptr<PartBuildStrategy>>
unpack_strategy(Napi::Env env, Napi::Object strategy_js, std::map<std::string, std::shared_ptr<SignalMeta>> const& signals )
{
  Napi::Array strategy_js_pair = strategy_js.As<Napi::Array>();
  uint32_t name_index = 0;
  uint32_t value_index = 1;
  std::string name = strategy_js_pair.Get(name_index).As<Napi::String>().Utf8Value();
  Napi::Object strategy_obj = strategy_js_pair.Get(value_index).ToObject();

  std::string type = strategy_obj.Get("type").As<Napi::String>().Utf8Value();
  double value = strategy_obj.Get("value").ToNumber().Int32Value();
  int max = strategy_obj.Get("max").ToNumber().Int32Value();
  int min = strategy_obj.Get("min").ToNumber().Int32Value();

  std::shared_ptr<SignalMeta> signal_meta = signals.at(name);
  std::unique_ptr<PartBuildStrategy> strategy = BuildStrategyStoreHouse::Create(type, signal_meta, value);

  return std::make_pair(name, std::move(strategy));
}

} //namespace can
