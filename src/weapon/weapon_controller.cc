#include <vector>
#include <iomanip>
#include <iostream>
#include <tools/enumerate.h>

#include "can/message.h"
#include "intelligence-agency.h"
#include "weapon_controller.h"
#include "tools/utils.h"

namespace can {

WeaponController::WeaponController(std::string const &name)
        : name_(name),
          frame_property_({}),
          timer_(std::bind(&WeaponController::Auto_, this))
{
  timer_.SetSingleShot(false);
  timer_.SetInterval(Timer::Interval(200));
  InitializeReceiver();
}

WeaponController::~WeaponController()
{
  timer_.Stop();
  UnInitializeReceiver();
}

std::string &
WeaponController::Name()
{
  return name_;
}

void
WeaponController::Fire()
{
  timer_.Start(true);
  firing_ = true;
}

void
WeaponController::CeaseFire()
{
  timer_.Stop();
  firing_ = false;
}

std::pair< std::vector<Message>,  std::vector<Message>>
WeaponController::AcquireReceiveData()
{

  std::vector<Message> known;
  std::vector<Message> unknown;
  receive_mtx.lock();
  std::cout << "WeaponController::AcquireReceiveData  recv_messages.size is " << recv_messages.size() << "\n";
  for(auto& p : recv_messages) {
    stringstream_.clear();
    stringstream_.str("");
    for(int i = 0; i < p.second.dlc; ++i) {
      stringstream_ << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(p.second.raw[i]) << " ";
    }
    p.second.data = stringstream_.str();

    auto iter = mp_message_meta_.find(p.first);

    if(iter != mp_message_meta_.end()) {
      p.second.name = iter->second->name;
      for(std::string signal_name : iter->second->signal_names) {
        std::shared_ptr<SignalMeta> signal_meta = mp_signal_meta_.at(signal_name);
        double signal_value = transform(p.second.raw, p.second.dlc, signal_meta);
        p.second.signals.push_back(Signal{signal_name, signal_value});
      }
      std::cout << " know " << p.second.name << " id " << p.second.id << "\n";
      known.push_back(p.second);
    } else {
      unknown.push_back(p.second);
    }

  }
  recv_messages.clear();

  receive_mtx.unlock();

  return std::make_pair(known, unknown);
};

void
WeaponController::InitializeReceiver() {
  if(!is_receiving_) {
    receive_t_ = std::thread(
            &WeaponController::onStartReceive, this);
    is_receiving_ = true;
  }
}

bool WeaponController::IsReceiving() {
  return is_receiving_;
}

void
WeaponController::UnInitializeReceiver() {
  is_receiving_ = false;
  receive_t_.detach();
}

void
WeaponController::OnReceiveMessages(std::vector<Message> & messages)
{
  for (auto& c : messages) {
    std::cout << " messge id:" << c.id << " data:" << c.raw << "\n";
    recv_messages.emplace(c.id, c);
  }
}

void
WeaponController::SetMetaData(std::map<uint32_t, std::shared_ptr<MessageMeta>> const& mp_message_meta,
                 std::map<std::string, std::shared_ptr<SignalMeta>> const& mp_signal_meta,
                 std::map<std::string, std::shared_ptr<PartBuildStrategy>> const& mp_strategies)
{
  for (auto c : mp_message_meta) {
    mp_message_meta_.emplace(c.first, c.second);
  }

  for (auto c : mp_signal_meta) {
    mp_signal_meta_.emplace(c.first, c.second);
  }

  for (auto c : mp_strategies) {
    mp_strategies_.emplace(c.first, c.second);
  }

}

void
WeaponController::loadAmmo(std::vector<uint32_t> msg_ids)
{
  for(uint32_t msg_id : msg_ids) {
    loadAmmo(msg_id);
//    std::shared_ptr<MessageMeta> message_meta = mp_message_meta_.at(msg_id);
//    std::vector<std::unique_ptr<AmmoPartGenerator>> part_generators;
//    for(std::string signal_name : message_meta->signal_names) {
//      std::shared_ptr<PartBuildStrategy> build_strategy = mp_strategies_.at(signal_name);
//      part_generators.emplace_back(std::make_unique<AmmoPartGenerator>(build_strategy));
//    }
//    ammo_generators_.emplace_back(std::make_unique<AmmoGenerator>(message_meta, part_generators));
  }
}

void
WeaponController::loadAmmo(uint32_t msg_id)
{
  std::shared_ptr<MessageMeta> message_meta = mp_message_meta_.at(msg_id);
  std::vector<AmmoPartGenerator> part_generators;
  for(std::string signal_name : message_meta->signal_names) {
    std::shared_ptr<PartBuildStrategy> build_strategy = mp_strategies_.at(signal_name);
    part_generators.emplace_back(AmmoPartGenerator(build_strategy));
  }
  std::unique_ptr<AmmoGenerator> generator = std::make_unique<AmmoGenerator>(message_meta, part_generators);
  ammo_generators_.emplace_back(std::move(generator));
}

void
WeaponController::unloadAmmo(uint32_t msg_id)
{
  std::vector<std::unique_ptr<AmmoGenerator>>::iterator it =
          std::find_if(ammo_generators_.begin(), ammo_generators_.end(),
                       [&](std::unique_ptr<AmmoGenerator> & obj){ return obj->MsgId() == msg_id;});

  if(it != ammo_generators_.end()) {
    ammo_generators_.erase(it);
  }

}

void
WeaponController::SetStrategy(std::string signal_name,
                              std::shared_ptr<PartBuildStrategy> strategy)
{
  uint32_t msg_id = 0;
  auto msg_iterator = std::find_if(std::begin(mp_message_meta_),
                         std::end(mp_message_meta_),
                         [signal_name] (std::pair<uint32_t, std::shared_ptr<MessageMeta>> const& entry)
                         {
                           std::vector<std::string> v = entry.second->signal_names;
                           return std::find(v.begin(), v.end(), signal_name) != v.end();
                         });

  if (msg_iterator != std::end(mp_message_meta_)) {
    std::cout << "WeaponController::SetStrategy   find msg_id !!!!  -> " << msg_iterator->first << "\n";
    msg_id = msg_iterator->first;
  } else {
    std::cout << "WeaponController::SetStrategy  错误 error !!!!!!!!!!!!!!!!!!, didn't find msg_id" << "\n";
    return;
  }

  mp_strategies_[signal_name] = strategy;

  std::vector<std::unique_ptr<AmmoGenerator>>::iterator it =
          std::find_if(ammo_generators_.begin(), ammo_generators_.end(),
                       [&](std::unique_ptr<AmmoGenerator> & obj){ return obj->MsgId() == msg_id;});

  if (it != ammo_generators_.end())
  {
    (*it)->SetStrategy(signal_name, strategy);
  }
}

void
WeaponController::Auto_() {
  std::vector<Ammo> ammos;
  for(auto& e : ammo_generators_) {
    ammos.emplace_back(e->Generate());
  }
  Shoot(ammos);
//  Shoot(ammo_generators_.Generate());
}

} //namespace can
