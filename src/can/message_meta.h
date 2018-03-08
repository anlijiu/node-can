#ifndef CAN_MESSAGE_META_H
#define CAN_MESSAGE_META_H


#include "can/signal_meta.h"
#include <vector>

namespace can {

struct MessageMeta {
  std::string name;
  uint32_t id;
  uint8_t dlc;
  std::vector<std::string> signal_names;

  bool operator==(const MessageMeta & obj2) const
  {
    if(this->name.compare(obj2.name) == 0)
      return true;
    else
      return false;
  }
};

}// namespace can
#endif //CAN_MESSAGE_META_H

