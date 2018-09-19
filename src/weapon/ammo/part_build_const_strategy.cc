//
// Created by anlijiu on 18-1-19.
//

#include "tools/utils.h"
// #include "tools/can_bits.h"
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


int PartBuildConstStrategy::Generate(uint8_t * dest)
{
  auto signal_meta = GetSignalMeta();
  uint64_t value = static_cast<uint64_t>(value_/signal_meta->scaling);
  std::cout << " PartBuildConstStrategy::Generate in ,name is " << Name() << " value is " << value << "\n";
  ::insert(dest, signal_meta->start_bit, signal_meta->length, value, MOTOROLA);
  // int status = data_pack(dest, value, signal_meta->start_bit, signal_meta->length);
  return 1; 
}

} // namespace can
