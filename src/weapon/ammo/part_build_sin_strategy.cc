//
// Created by anlijiu on 18-1-19.
//
#include <cmath>
#include "tools/utils.h"
// #include "tools/can_bits.h"
#include "part_build_sin_strategy.h"

namespace can {


PartBuildSinStrategy::PartBuildSinStrategy(std::shared_ptr<SignalMeta> signal_meta)
        : PartBuildStrategy(signal_meta)
{}

BuildStrategyType
PartBuildSinStrategy::Type()
{
  return type_;
}

int PartBuildSinStrategy::Generate(uint8_t * dest)
{
  ++degree;
  if(degree > 360) degree = 0;
  auto signal_meta = GetSignalMeta();
  double half = (signal_meta->maximum - signal_meta->minimum) / 2;
  uint64_t value = static_cast<uint64_t>( (sin (degree*PI/180) * half + half)/signal_meta->scaling);
  std::cout << " PartBuildSinStrategy::Generate in ,name is " << Name() << " value is " << value << "\n";
  ::insert(dest, signal_meta->start_bit, signal_meta->length, value, MOTOROLA);
  // int status = data_pack(dest, value, signal_meta->start_bit, signal_meta->length);
  return 1; 
}

} // namespace can
