//
// Created by anlijiu on 18-1-19.
//
#ifndef CANALYST_SIGNAL_BUILDER_CONST_STRATEGY_H
#define CANALYST_SIGNAL_BUILDER_CONST_STRATEGY_H

#include "can/signal_build_strategy.h"

namespace can {

class SignalBuildConstStrategy : public SignalBuildStrategy {
public:
  SignalBuildConstStrategy(SignalMetaTp const& signal_meta) : SignalBuildStrategy(signal_meta) {}
  SignalBuildConstStrategy(SignalMetaTp const& signal_meta, double value)
          : SignalBuildStrategy(signal_meta), value_(value) {}

  std::string Name() { return name_; }
  void Value(double value) {
    value_ = value;
  }

  std::bitset<64> generate();
private:
  double value_ = 0;
  const std::string name_ = "const";
};


} //namespace can
#endif //CANALYST_SIGNAL_BUILDER_CONST_STRATEGY_H
