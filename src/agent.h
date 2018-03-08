#ifndef CAN_AGENT_H
#define CAN_AGENT_H

#include <vector>
#include <memory>
#include <napi.h>
#include "weapon/weapon_controller.h"

namespace can {

class Agent: public Napi::ObjectWrap<Agent>
{
public:
  static Napi::FunctionReference constructor;
  explicit Agent(const Napi::CallbackInfo& callbackInfo); // constructor
  ~Agent();

  static void Init(Napi::Env& env, Napi::Object& target);

  void SyncMetaData(const Napi::CallbackInfo&);

  Napi::Value Weapons(const Napi::CallbackInfo&);
  void SetWeapon(const Napi::CallbackInfo&, const Napi::Value&);
  void SetAmmoMetas(const Napi::CallbackInfo&, const Napi::Value&);

  void Start(const Napi::CallbackInfo&);
  void Stop(const Napi::CallbackInfo&);

  void Fire(const Napi::CallbackInfo&);
  void CeaseFire(const Napi::CallbackInfo&);

  void SetAmmos(const Napi::CallbackInfo&);
  void AddAmmo(const Napi::CallbackInfo&);
  void RemoveAmmo(const Napi::CallbackInfo&);
  void SetAmmoBuildStrategy(const Napi::CallbackInfo&);


  Napi::Value AcquireReceiveData(const Napi::CallbackInfo& info);

  // 目前napi　还没办法从其他线程ｃａｌｌｂａｃｋ　主线程，　等他们搞定了再上
  // 所以目前用ｊｓ端轮寻调用AcquireReceiveData 请求ｃａｎ线发上来的数据

//  Napi::Value OnMessagesReceived(const Napi::CallbackInfo& info);
//  Napi::Value OnUnknowMessagesReceived(const Napi::CallbackInfo& info);
//
//  void ReceiveMessages(std::vector<Message> const& messages);
//  void ReceiveUnknownMessages(std::vector<Message> const& messages);

private:
  typedef std::vector<Napi::FunctionReference> Handlers;

  Handlers msg_handlers;
  Handlers unknown_msg_handlers;

  std::unique_ptr<WeaponController> weapon_controller_;
  std::string weapon_name_ = "canalystii";
  int weapon_rate_ = 500;
};
} //namespace can

#endif
