#ifndef CAN_SIGNAL_TRANSFORMER_H
#define CAN_SIGNAL_TRANSFORMER_H

#include <bitset>
#include "signal_meta.h"

namespace can {

class SignalBuildStrategy {
public:

  explicit SignalBuildStrategy(SignalMetaTp const& signal_meta): signal_meta_(signal_meta) {}

  virtual ~SignalBuildStrategy() {}

  virtual std::bitset<64> generate() = 0;

  SignalMetaTp GetSignalMetaTp() { return signal_meta_; }

private:
  SignalMetaTp signal_meta_ = nullptr;
};

} // namespace can

#endif //CAN_SIGNAL_TRANSFORMER_H
