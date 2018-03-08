#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include <cstdint>
#include <bitset>
#include <vector>
#include "can/signal.h"

namespace can {

/**
 * 接收到的数据帧
 * */
struct Message {
  uint32_t id;
  uint8_t dlc;
  uint8_t raw[8];
  std::string data;

  bool known;
  std::string name;
  std::vector<Signal> signals;
};

} // namespace can
#endif //CAN_MESSAGE_META_H

