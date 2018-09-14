//
// Created by anlijiu on 1/23/18.
//
#include <can/message_meta.h>
#include <tools/meta_helper.h>
#include "agent.h"
#include "tools/validate_arguments.h"
#include "tools/enumerate.h"
#include "can/signal_meta.h"
#include "weapon/ammo/part_build_strategy.h"
#include "store_house.h"
#include "intelligence-agency.h"

using namespace Napi;
using namespace std;

namespace can {

FunctionReference Agent::constructor;

void
Agent::Init(Napi::Env &env, Napi::Object &target) {
  cout << "Agent::Init" << "\n";
  Napi::HandleScope scope(env);
  Function ctor = DefineClass(
          env,
          "Agent",
          {
                  InstanceAccessor("weapon", nullptr, &Agent::SetWeapon),
                  InstanceAccessor("supportWeapons", &Agent::Weapons, nullptr),
                  InstanceAccessor("ammoMetas", nullptr, &Agent::SetAmmoMetas),
                  InstanceMethod("syncMetaData", &Agent::SyncMetaData),
                  InstanceMethod("acquireReceivedMessage", &Agent::AcquireReceiveData),
//                  InstanceMethod("onMessagesReceived", &Agent::OnMessagesReceived),
//                  InstanceMethod("onUnknownMessagesReceived", &Agent::OnUnknowMessagesReceived),
                  InstanceMethod("fire", &Agent::Fire),
                  InstanceMethod("ceaseFire", &Agent::CeaseFire),
                  InstanceMethod("start", &Agent::Start),
                  InstanceMethod("stop", &Agent::Stop),
                  InstanceMethod("addAmmo", &Agent::AddAmmo),
                  InstanceMethod("setAmmos", &Agent::SetAmmos),
                  InstanceMethod("removeAmmo", &Agent::RemoveAmmo),
                  InstanceMethod("setAmmoPartBuildStrategy", &Agent::SetAmmoBuildStrategy),
          });
  constructor = Persistent(ctor);
  constructor.SuppressDestruct();
  target.Set("Agent", ctor);
}

Agent::Agent(const CallbackInfo &info)
        : ObjectWrap(info) {
  cout << "Agent::Agent" << "\n";
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (info.Length() == 1) {
    Napi::Object options = info[0].As<Napi::Object>();
    if (options.Has("weapon")) {
      this->weapon_name_ = options.Get("weapon").As<Napi::String>().Utf8Value();
    }
    if (options.Has("weaponRate")) {
      this->weapon_rate_ = options.Get("weaponRate").As<Napi::Number>().Int32Value();
    }
  }

  weapon_controller_ = WeaponStoreHouse::Create(weapon_name_, weapon_name_, weapon_rate_);

  if (!weapon_controller_) {
    throw Error::New(info.Env(), "weapon has not been initialized properly");
  }

  IntelligenceAgency::RegisterAgent(this);
}

Agent::~Agent() {
  cout << "Agent::~Agent" << "\n";
  IntelligenceAgency::UnRegisterAgent(this);
}

/**
 * app启动时同步js数据到c++
 *
 * @param messages meta
 * @param signals meta
 * @param strategies
 * @return void
 */
void 
Agent::SyncMetaData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsArray() || !info[1].IsArray() || !info[2].IsArray()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return;
  }

  Napi::Array messages_js = info[0].As<Napi::Array>();
  Napi::Array signals_js = info[1].As<Napi::Array>();
  Napi::Array strategies_js = info[2].As<Napi::Array>();

  std::map<uint32_t, std::shared_ptr<MessageMeta>> messages;
  std::map<std::string, std::shared_ptr<SignalMeta>> signals;
  std::map<std::string, std::shared_ptr<PartBuildStrategy>> strategies;

  for (unsigned int i = 0; i < messages_js.Length(); ++i) {
    Napi::Object message_meta_obj = Napi::Object(env, messages_js.Get(i));
    auto message_meta = unpack_message_meta(env, message_meta_obj);
    messages[message_meta.first] = message_meta.second;
  }

  for (unsigned int i = 0; i < signals_js.Length(); ++i) {
    Napi::Object signal_meta_obj = Napi::Object(env, signals_js.Get(i));
    auto signal_meta = unpack_signal_meta(env, signal_meta_obj);
    signals[signal_meta.first] = signal_meta.second;
  }

  for (unsigned int i = 0; i < strategies_js.Length(); ++i) {
    Napi::Object strategy_obj = Napi::Object(env, strategies_js.Get(i));
    auto strategy_pair = unpack_strategy(env, strategy_obj, signals);
    strategies[strategy_pair.first] = strategy_pair.second;
  }

  weapon_controller_->SetMetaData(messages, signals,strategies);
}

Napi::Value
Agent::Weapons(Napi::CallbackInfo const& info ) {
  cout << "Agent::Weapons" << "\n";

  auto keys = StoreHouse<std::string, WeaponController>::Keys();

  Napi::Array arr = Napi::Array::New(info.Env(), keys.size());

  for (int i = 0; i < keys.size(); ++i) {
    arr.Set(i, Napi::String::New(info.Env(), keys.at(i)));
  }

  return arr;
}

void
Agent::SetWeapon(Napi::CallbackInfo const& info , Napi::Value const& value) {
  cout << "Agent::SetWeapon" << "\n";

  weapon_name_ = value.As<Napi::String>().Utf8Value();
  weapon_controller_ = WeaponStoreHouse::Create(weapon_name_, weapon_name_, weapon_rate_);
}

void
Agent::SetAmmoMetas(Napi::CallbackInfo const& info , Napi::Value const& value) {
  cout << "Agent::SetAmmoMetas start" << "\n";

//  Napi::Array jsArr = value.As<Napi::Array>();
//  std::vector<std::shared_ptr<MessageMeta>> messages;
//  for (unsigned int i = 0; i < jsArr.Length(); ++i) {
//    cout << "Agent::SetAmmoMetas   index " << i << '\n';
//    Napi::Object message_meta_obj = Napi::Object(value.Env(), jsArr.Get(i));
//    messages.push_back(unpack_message_meta(value.Env(), message_meta_obj));
//  }
//
//  weapon_controller_->SetAmmoMetas(messages);

  std::cout << "Agent::SetAmmoMetas end " << '\n';
}

void
Agent::Start(Napi::CallbackInfo const& info) {
  cout << "Agent::Start" << '\n';
  weapon_controller_->Fire();
}

void
Agent::Stop(Napi::CallbackInfo const& info) {
  cout << "Agent::Stop" << '\n';
  weapon_controller_->CeaseFire();
}


void 
Agent::Fire(Napi::CallbackInfo const& info) {
  cout << "Agent::Fire" << '\n';
  weapon_controller_->Fire();
}

void
Agent::CeaseFire(Napi::CallbackInfo const& info) {
  cout << "Agent::CeaseFire" << '\n';
  weapon_controller_->CeaseFire();
}

void
Agent::SetAmmos(const Napi::CallbackInfo& info) {
  cout << "Agent::SetAmmos " << '\n';
  std::vector<uint32_t > msg_ids;

  Napi::Array msg_ids_js = info[0].As<Napi::Array>();
  for (unsigned int i = 0; i < msg_ids_js.Length(); ++i) {
    uint32_t msg_id = msg_ids_js.Get(i).As<Napi::Number>().Uint32Value();
    msg_ids.push_back(msg_id);
  }

  weapon_controller_->loadAmmo(msg_ids);
}

void
Agent::AddAmmo(const Napi::CallbackInfo& info) {
  cout << "Agent::AddAmmo" << '\n';

  uint32_t msg_id = info[0].As<Napi::Number>().Uint32Value();

  weapon_controller_->loadAmmo(msg_id);

}

void
Agent::RemoveAmmo(const Napi::CallbackInfo& info) {

  uint32_t msg_id = info[0].As<Napi::Number>().Uint32Value();
  cout << "Agent::RemoveAmmo" << msg_id <<  '\n';
  weapon_controller_->unloadAmmo(msg_id);
}

void
Agent::SetAmmoBuildStrategy(const Napi::CallbackInfo& info) {
  cout << "Agent::SetAmmoBuildStrategy" << '\n';
  Napi::Env env = info.Env();
  Napi::Object strategy_js = info[0].As<Napi::Object>();
  auto strategy_pair = unpack_strategy(env, strategy_js,  weapon_controller_->Signals());
  weapon_controller_->SetStrategy(strategy_pair.first, strategy_pair.second);
}


Napi::Value
Agent::AcquireReceiveData(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  Napi::Array messages_js = Napi::Array::New(env);
  Napi::Array unknown_messages_js = Napi::Array::New(env);
  auto m = weapon_controller_->AcquireReceiveData();
  int i = 0;
  for(auto e : enumerate(m.first)) {
    const auto& index = e.first;
    const auto& message = e.second;
    Napi::Object message_js = Napi::Object::New(env);
    Napi::Array signals_js = Napi::Array::New(env);
    message_js.Set("name", message.name);
    message_js.Set("id", message.id);
    message_js.Set("raw", message.data);
    i = 0;
    for(auto s : enumerate(message.signals)) {
      Napi::Object signal_js = Napi::Object::New(env);
      signal_js.Set("value", s.second.value);
      signal_js.Set("name", s.second.name);
      signals_js.Set(i, signal_js);
      ++i;
    }
    message_js.Set("signals", signals_js);
    messages_js.Set(index, message_js);
  }
  // for(const auto& message: m.first) {
  //   Napi::Object message_js = Napi::Object::New(env);
  //   Napi::Object signals_js = Napi::Object::New(env);
  //   message_js.Set("name", message.name);
  //   message_js.Set("id", message.id);
  //   message_js.Set("raw", message.data);
  //   for(auto s : message.signals) {
  //     signals_js.Set(s.name, s.value);
  //     std::cout << "signal name is :" << s.name << s.value ;
  //   }
  //   message_js.Set("signals", signals_js);
  //   messages_js.Set(message.name, message_js);
  // }
  
  for(auto e : enumerate(m.second)) {
    const auto& index = e.first;
    const auto& message = e.second;
    Napi::Object unknown_message_js = Napi::Object::New(env);
    unknown_message_js.Set("raw", message.data);
    unknown_message_js.Set("id", message.id);
    unknown_messages_js.Set(index, unknown_message_js);
  }
  // for(const  auto& message: m.second ) {
  //   Napi::Object unknown_message_js = Napi::Object::New(env);
  //   unknown_message_js.Set("raw", message.data);
  //   unknown_messages_js.Set(message.id, unknown_message_js);
  // }

  obj.Set("messages", messages_js);
  obj.Set("unknowns", unknown_messages_js);

  return obj;
}
//
//
//Napi::Value Agent::OnMessagesReceived(const Napi::CallbackInfo& info) {
//  Napi::Env env = info.Env();
//  std::cout << "Agent::OnMessagesReceived,  info[0].IsFunction() -> " << info[0].IsFunction();
//  Napi::Function handler = info[0].As<Napi::Function>();
//
//  std::cout << " Agent::onMessageChange  handler is   " << handler << "\n";
//  msg_handlers.push_back(Napi::Reference<Napi::Function>::New(handler, 1));
//
//  return Napi::Boolean::New(env, true);
//}
//
//Napi::Value Agent::OnUnknowMessagesReceived(Napi::CallbackInfo const& info) {
//  Napi::Env env = info.Env();
//  Napi::Function handler = info[0].As<Napi::Function>();
//  handler.Call( { Napi::String::New(env, "hahahahahahahaasdlfka") } );
//  std::cout << "Agent::OnUnknowMessagesReceived, handler -> " << handler;
//
//  unknown_msg_handlers.push_back(Napi::Reference<Napi::Function>::New(handler, 1));
//
//  return Napi::Boolean::New(env, true);
//}
//
//
//void
//Agent::ReceiveMessages(std::vector<Message> const& messages)
//{
//
//  std::cout << "IntelligenceAgency::MessagesChanged in  msg_handlers size is " << msg_handlers.size();
//
//  if(msg_handlers.empty()) return;
//
//  Napi::Env env = msg_handlers[0].Env();
//
//  Napi::Object messages_js = Napi::Object::New(env);
//  Napi::Object signals_js = Napi::Object::New(env);
//  for(const auto& message: messages) {
//    Napi::Object message_js = Napi::Object::New(env);
//    message_js.Set("name", message.name);
//    message_js.Set("id", message.id);
//    message_js.Set("raw", message.data);
//    for(auto s : message.signals) {
//      signals_js.Set(s.name, s.value);
//      std::cout << "signal name is :" << s.name << s.value ;
//    }
//    message_js.Set("signals", signals_js);
//    messages_js.Set(message.name, message_js);
//  }
//
//
//  for(Message message : messages) {
//    std::cout << "message data is :" << message.data << "\n";
//  }
//
//  std::vector<Napi::Function> list;
//  for (Napi::FunctionReference &ref: msg_handlers)
//    list.push_back(ref.Value());
//
//  for (Napi::Function &func: list) {
//    std::cout << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh 111 " << func << "\n";
//    func.Call({messages_js});
//    std::cout << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh 222 \n";
//  }
//}
//
//void
//Agent::ReceiveUnknownMessages(std::vector<Message> const& messages)
//{
//  std::cout << "Agent::ReceiveUnknownMessages  messages size is " << messages.size() << "\n";
//  if(unknown_msg_handlers.empty()) return;
//
////  std::vector<Napi::Function> list;
//  for (Napi::FunctionReference& ref: unknown_msg_handlers) {
//    std::cout << "Agent::ReceiveUnknownMessages 1111\n" ;
//
//    Napi::Function func = ref.Value();
//    std::cout << "Agent::ReceiveUnknownMessages 123\n" ;
//    Napi::Env env = func.Env();
//    std::cout << "Agent::ReceiveUnknownMessages 234\n" ;
//    Napi::Object messages_js = Napi::Object::New(env);
//    std::cout << "Agent::ReceiveUnknownMessages 345\n" ;
////    Napi::Object messages_js = Napi::Object::New(env);
//    std::cout << "Agent::ReceiveUnknownMessages 222\n" ;
//    for(const auto& message: messages) {
//      std::cout << "Agent::ReceiveUnknownMessages 333\n" ;
//      Napi::Object message_js = Napi::Object::New(env);
//      message_js.Set("raw", message.data);
//
//      messages_js.Set(message.id, message_js);
//      std::cout << "Agent::ReceiveUnknownMessages 444\n" ;
//    }
//    std::cout << "Agent::ReceiveUnknownMessages 555\n" ;
//    std::cout << "Agent::ReceiveUnknownMessages func is " << func;
//    func.Call( { messages_js });
//  }
//}

} // namespace can
