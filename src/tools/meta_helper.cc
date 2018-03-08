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
  uint32_t start = std::stoi(signal_meta_obj.Get("startbit").As<Napi::String>().Utf8Value());
  uint32_t length = std::stoi(signal_meta_obj.Get("bitlength").As<Napi::String>().Utf8Value());
  std::string endianess = signal_meta_obj.Get("endianess").As<Napi::String>().Utf8Value();
  double scaling = std::stod(signal_meta_obj.Get("scaling").As<Napi::String>().Utf8Value());
  double offset = std::stod(signal_meta_obj.Get("offset").As<Napi::String>().Utf8Value());
  double minimum = std::stod(signal_meta_obj.Get("minimum").As<Napi::String>().Utf8Value());
  double maximum = std::stod(signal_meta_obj.Get("maximum").As<Napi::String>().Utf8Value());
  bool is_signed;
  std::istringstream is(signal_meta_obj.Get("signed").As<Napi::String>().Utf8Value());
  is >> std::boolalpha >> is_signed;
  std::string units = signal_meta_obj.Get("units").As<Napi::String>().Utf8Value();
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
  std::cout << " unpack_message_meta in " << '\n';
  std::string name = message_meta_obj.Get("name").As<Napi::String>().Utf8Value();
  uint32_t id = static_cast<uint32_t>(std::stoi(message_meta_obj.Get("id").As<Napi::String>().Utf8Value()));
  uint8_t dlc = static_cast<uint8_t>(std::stoi(message_meta_obj.Get("dlc").As<Napi::String>().Utf8Value()));
  auto msg_tp = std::make_shared<MessageMeta>(MessageMeta{
          name,
          id,
          dlc,
          std::vector<std::string>()
  });
  Napi::Value signalValue = message_meta_obj.Get("signal");
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

  std::cout << "unpack_strategy type: " << type << " value :" << value << " name " << name;
  return std::make_pair(name, std::move(strategy));
}

} //namespace can
