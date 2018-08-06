#ifndef WEAPON_CONTROLLER_H
#define WEAPON_CONTROLLER_H

#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <condition_variable>
//#include <thread>
#include "weapon/ammo/ammo_generator.h"
#include "tools/timer.h"
#include "can/message.h"
#include "weapon/ammo/ammo.h"
#include "can/signal_meta.h"
#include "store_house.h"

#include "can/can.h"
#include "tools/property.h"
#include "gatling_barrel.h"

namespace can {

class WeaponController {
public:
  WeaponController() = default;
  WeaponController(std::string const& name);
  ~WeaponController();
  virtual int Initialize() = 0;

  virtual bool Release() = 0;

  virtual void Fire();

  virtual void CeaseFire();

  void InitializeReceiver();
  bool IsReceiving();
  void UnInitializeReceiver();


  std::string& Name();

  void SetMetaData(std::map<uint32_t, std::shared_ptr<MessageMeta>> const&,
                std::map<std::string, std::shared_ptr<SignalMeta>> const&,
                std::map<std::string, std::shared_ptr<PartBuildStrategy>> const&);

  void SetStrategy(std::string signal_name, std::shared_ptr<PartBuildStrategy> strategy);

  void loadAmmo(std::vector<uint32_t> msg_ids);
  void loadAmmo(uint32_t msg_id);
  void unloadAmmo(uint32_t msg_id);

  virtual int AddRadarListener(std::function<void(const can_frame&)> const& listener) {
    return frame_property_.on_change().connect(listener);
  };

  virtual void RemoveRadarListener(int id) {
    frame_property_.on_change().disconnect(id);
  }

  const std::map<std::string, std::shared_ptr<SignalMeta>>& Signals() {
    return mp_signal_meta_;
  };

  std::pair<std::vector<Message>,  std::vector<Message>> AcquireReceiveData();

protected:
  virtual std::tuple<bool, int> Shoot(Ammo const& ammo) = 0;
  virtual std::tuple<bool, int> Shoot(std::vector<Ammo> const& ammos) = 0;
  virtual void onStartReceive() = 0;
  virtual void onStopReceive() = 0;

  void OnReceiveMessages(std::vector<Message> & messages);

  bool is_receiving_ = false;
  bool is_receive_t_freeze_ = true;
  std::condition_variable receive_cv_;
  std::mutex receive_mtx;
  std::thread receive_t_;
private:

  std::map<uint32_t, std::shared_ptr<MessageMeta>> mp_message_meta_;
  std::map<std::string, std::shared_ptr<SignalMeta>> mp_signal_meta_;
  std::map<std::string, std::shared_ptr<PartBuildStrategy>> mp_strategies_;
  std::vector<uint32_t> loaded_ammos_;
  std::stringstream stringstream_;

  Property<can_frame> frame_property_;//TODO
  std::string name_;
  Timer timer_;
  // GatlingBarrel ammo_generators_;
  std::vector<std::unique_ptr<AmmoGenerator>> ammo_generators_;
  bool firing_ = false;

  std::map<uint32_t, Message> recv_messages;

  void Auto_(void);
};

using WeaponStoreHouse = StoreHouse<std::string, WeaponController>;

} //namespace can


#endif /* WEAPON_CONTROLLER_H */
