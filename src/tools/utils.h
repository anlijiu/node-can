#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include <bitset>
#include <memory>
#include "can/signal_meta.h"
#include "tools/bits.h"

namespace can {

std::string currentDateAndTime();

void
printBits(size_t const size, void const *const ptr);

template <size_t max>
std::bitset<max> reverse(std::bitset<max> const &bset)
{
  std::bitset<max> reverse;
  for (size_t iter = 0; iter < max; ++iter)
  {
    reverse[iter] = bset[max - iter - 1];
  }
  return reverse;
}

long extractBits(const uint8_t data[], short offset, size_t length, ByteOrder order);

std::bitset<64> transformer(double value, std::shared_ptr<SignalMeta> signal_meta);

uint64_t transform(double value, std::shared_ptr<SignalMeta> signal_meta);

double transform(const uint8_t data[],  size_t length, std::shared_ptr<SignalMeta> signal_meta);

unsigned char* bitstoByteArray(std::bitset<64> const& bits);
} //namespace can

#endif /* CAN_UTILS_H */
