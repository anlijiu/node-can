#include <cstdio>
#include <cstdint>
#include <cassert>
#include <iostream>
#include "utils.h"

#include "3rdparty/endian/endianness_config.h"
namespace can {

//assumes little endian
void printBits(size_t const size, void const *const ptr) {
  unsigned char *b = (unsigned char *) ptr;
  unsigned char byte;
  int i, j;
  for (i = 0; i <= size - 1; ++i) {
    for (j = 0; j <= 7; ++j) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
    printf(" ");
  }

  printf("\n");
}

long extractBits(const uint8_t data[], short offset, size_t length, ByteOrder order) {
  long val = 0;
  printf("\ndata is ");
  printBits(sizeof(&data), data);
  printf("\nval is: \n");
  if (order == kBigEndian) {
    for (int i = 0; i < length; i++) {
      int bitNr = i + offset;
      val |= ((data[bitNr >> 3] >> (bitNr & 0x07)) & 1) << i;
      printBits(sizeof(val), &val);
    }
  } else {
    for (int i = 0; i < length; i++) {
      int bitNr = offset + length - i - 1;
      val |= ((data[bitNr >> 3] >> (7 - (bitNr & 0x07))) & 1) << i;
      printBits(sizeof(val), &val);
    }
  }

  return val;

}

//std::bitset<64> reverse(std::bitset<64> const &bset)
//{
//  std::bitset<64> reverse;
//  for (size_t iter = 0; iter != 64; ++iter)
//  {
//    reverse[iter] = bset[64 - iter - 1];
//  }
//  return reverse;
//}

std::bitset<64> transformer(double value, std::shared_ptr<SignalMeta> signal_meta) {
  // uint64_t raw = reinterpret_cast<unsigned long long>(value / signal_meta->scaling);
  std::cout << "  transformer value is "  << value << "   scaling is " << signal_meta->scaling << "\n";
  uint64_t raw = to_big_endian64((uint64_t)(value / signal_meta->scaling));
  std::bitset<64> bitset(raw);
  std::cout << "raw:" << raw << "before    bitset <<= " << signal_meta->start_bit << " , bitset is " << bitset << "\n";
  bitset  <<= (signal_meta->start_bit-1);
  std::cout << "bitset is " << bitset << "\n";
  return bitset;
}

uint64_t transform(double value, std::shared_ptr<SignalMeta> signal_meta) {
  uint64_t raw = to_big_endian64((uint64_t)(value / signal_meta->scaling));
  int offset = (0xfffffffffffffff0 & signal_meta->start_bit) - (signal_meta->start_bit & 0x07) + signal_meta-> length - 1;
  std::cout << "\n " << signal_meta->name << " raw is " << raw << " transform  offset is " << offset;
  raw = offset > 0 ? raw >> offset : raw << abs(offset);
  return raw;
}

double transform(const uint8_t data[],  size_t length, std::shared_ptr<SignalMeta> signal_meta) {
  long value = 0;

  for (int i = 0; i < length; i++) {
    int bitNr = i + signal_meta->start_bit;
    value |= ((data[bitNr >> 3] >> (bitNr & 0x07)) & 1) << i;
  }

  return value * signal_meta->scaling;
}

unsigned char* bitstoByteArray(std::bitset<64> const& bits) {
  uint8_t *bytes = new uint8_t[8]();
  for (int i = 0; i<64; i++){
    if (bits.test(i))  {
      //bitset中为little endian
      //而CAN总线需要 big endian
      bytes[i >> 3] |= 1 << (7 - i & 0x07);
//      bytes[7 - (i >> 3)] |= 1 << (i & 0x07);
    }
  }

  return bytes;
}
//unsigned char* bitstoByteArray(std::bitset<64> const& bits) {
//  assert(bits.size()%8 == 0);
//  int len=bits.size()/8;
//  unsigned char *bytes = new unsigned char[len];
//  for (unsigned j = 0; j < len;++j) {
//    bytes[j] = 0;
//  }
//  for (int i = 0; i<bits.size(); i++){
//    if (bits.test(i))  { bytes[i/8] |= 1<<(7-i%8); }
//  }
//
//
//  return bytes;
//}

} //namespace can
