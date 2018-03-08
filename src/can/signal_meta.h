#ifndef CAN_SIGNAL_META_H
#define CAN_SIGNAL_META_H

#include <memory>

#include "tools/bits.h"

namespace can {

struct SignalMeta {
    std::string name;
    uint32_t start_bit;
    uint32_t length;
    ByteOrder order;
    double scaling;
    double offset;
    double minimum;
    double maximum;
    bool is_signed;
    std::string units;
};

typedef std::shared_ptr<SignalMeta> SignalMetaTp;

} //namespace can

#endif //CAN_SIGNAL_META_H
