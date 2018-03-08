//
// Created by anlijiu on 18-1-25.
//

#ifndef CAN_GATLINGBARREL_H
#define CAN_GATLINGBARREL_H

#include <vector>
#include <memory>
#include <weapon/ammo/ammo_generator.h>

namespace can {

class GatlingBarrel : public std::vector<std::unique_ptr<AmmoGenerator>>
{
public:
  Ammo Generate();
private:
  int position;
};

} //namespace can

#endif //CAN_GATLINGBARREL_H
