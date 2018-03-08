//
// Created by anlijiu on 18-1-19.
//

#include "can/signal_build_const_strategy.h"
#include "tools/utils.h"

namespace can {
std::bitset<64> SignalBuildConstStrategy::generate() {
  std::bitset<64> bitset;
  auto signal_meta = GetSignalMetaTp();
  return transformer(value_, signal_meta);

}
} // namespace can
