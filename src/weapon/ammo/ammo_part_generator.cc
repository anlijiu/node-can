//
// Created by anlijiu on 18-1-24.
//

#include "ammo_part_generator.h"
#include "part_build_const_strategy.h"

namespace can {

AmmoPartGenerator::AmmoPartGenerator(std::shared_ptr<PartBuildStrategy> strategy)
        : strategy_(strategy)
{
}

uint64_t
AmmoPartGenerator::generate()
{
  return strategy_ ? strategy_->Generate() : 0;
}

BuildStrategyType
AmmoPartGenerator::StrategyType()
{
  return strategy_->Type();
}

void
AmmoPartGenerator::SetStrategy(std::shared_ptr<PartBuildStrategy> strategy)
{
  strategy_ = strategy;
}

} //namespace can
