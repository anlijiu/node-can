#ifndef CAN_SIGNAL_TRANSFORMER_H
#define CAN_SIGNAL_TRANSFORMER_H

#include <bitset>
#include "message_meta.h"

class MessageBuildStragegy {
public:
  MessageBuildStragegy(SignalMeta signal_meta) : signal_meta_(signal_meta) {}
  virtual ~MessageBuildStragegy() {}
  std::bitset<64> generate();

private:
  std::vector<SignalBuildStragety> strageties_;
};

#endif //CAN_SIGNAL_TRANSFORMER_H
