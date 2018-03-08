//
// Created by anlijiu on 18-1-25.
//

#include "gatling_barrel.h"

namespace can {

Ammo
GatlingBarrel::Generate()
{
  if(position >= size()) {
    position = 0;
  }
  Ammo ammo = this->at(position)->Generate();
  ++position;
  return ammo;
}

} //namespace can
