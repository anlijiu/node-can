#ifndef CAN_AMMO_PART_BUILDER_H
#define CAN_AMMO_PART_BUILDER_H

#include <bitset>
#include "part_build_strategy.h"
#include "build_strategy_type.h"

namespace can {


class AmmoPartGenerator {
public:
  AmmoPartGenerator(std::shared_ptr<PartBuildStrategy>);
  uint64_t generate();
  void SetStrategy(std::shared_ptr<PartBuildStrategy>);
  BuildStrategyType StrategyType();
  std::string Name() { return strategy_->Name(); }
private:
  std::shared_ptr<PartBuildStrategy> strategy_ = nullptr;
};

} //namespace can

#endif //CAN_AMMO_PART_BUILDER_H
