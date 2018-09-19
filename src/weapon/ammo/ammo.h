#ifndef CAN_AMMO_H
#define CAN_AMMO_H

#include <cstdint>
#include <bitset>

namespace can {

struct Ammo {
  uint32_t id;
  uint8_t dlc;
  uint8_t data[8];
};

} // namespace can
#endif //CAN_MESSAGE_META_H

