#ifndef CAN_AMMO_H
#define CAN_AMMO_H

#include <cstdint>
#include <bitset>

namespace can {

struct Ammo {
  uint32_t id;
  uint8_t dlc;
  uint64_t data;
};

} // namespace can
#endif //CAN_MESSAGE_META_H

