#ifndef CANALYSTII_CONTROLLER_H
#define CANALYSTII_CONTROLLER_H

#include <stdio.h>
#include <iostream>
#include <string>

#include "can/can.h"
#include "tools/property.h"
#include "weapon_controller.h"
#include "store_house.h"
#include "canalystii.h"

using std::cout;
using std::cin;

namespace can {

class CanalystiiController : public WeaponController {
  using WeaponRegister = WeaponStoreHouse::StoreHouseRegister<CanalystiiController, std::string, int>;
public:
  CanalystiiController();
  explicit CanalystiiController( std::string const& name, int rate);
  int Initialize() override ;

  bool Release() override ;

protected:
  std::tuple<bool, int> Shoot(Ammo const& ammo) override ;
  std::tuple<bool, int> Shoot(std::vector<Ammo> const& ammos) override;
  void onStartReceive() override;
  void onStopReceive() override;
private:


  CANalystii can_node_;
  std::thread receiver_t_;
  int rate_;

  inline const static WeaponRegister reg{ std::string("canalystii") };
};

} //namespace can


#endif /* CANALYSTII_CONTROLLER_H */
