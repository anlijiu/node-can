#include "can.h"


namespace can {

std::ostream& operator<<(std::ostream &out, can_frame value) {
    out << "can_id:" << value.can_id 
        << "\trtr:"  << std::hex << (unsigned int)value.rtr 
        << "\tcan_dlc:" << std::hex << (unsigned int)value.can_dlc 
        << "\tdata:";
    for(int i = 0; i < CAN_MAX_DLEN; ++i) {
        std::cout << std::hex << (unsigned int)value.data[i] << " ";
    }
    std::cout << '\n';
    return out;
}

} //namespace can
