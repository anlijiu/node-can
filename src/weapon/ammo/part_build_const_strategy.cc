//
// Created by anlijiu on 18-1-19.
//

#include "tools/utils.h"
#include "part_build_const_strategy.h"

namespace can {


PartBuildConstStrategy::PartBuildConstStrategy(std::shared_ptr<SignalMeta> signal_meta, double value)
        : PartBuildStrategy(signal_meta), value_(value)
{}

BuildStrategyType
PartBuildConstStrategy::Type()
{
  return type_;
}

void
PartBuildConstStrategy::SetValue(double value)
{
  value_ = value;
}


uint64_t PartBuildConstStrategy::Generate()
{
  std::cout << " PartBuildConstStrategy::Generate in ,name is " << Name() << " value is " << value_ << "\n";
  auto signal_meta = GetSignalMeta();
  return transform(value_, signal_meta);
}

} // namespace can
