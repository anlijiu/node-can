#ifndef CAN_SIGNAL_TRANSFORMER_H
#define CAN_SIGNAL_TRANSFORMER_H

#include <bitset>

class SignalTransformer {
  std::bitset<64> transformer(double value);

private:
  SignalMeta signal_meta_;
}

#endif //CAN_SIGNAL_TRANSFORMER_H
