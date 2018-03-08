#ifndef CAN_CAN_H
#define CAN_CAN_H

#define CAN_MAX_DLEN 8

#include <iostream>
#include <cstring>

namespace can {


/**
 * struct can_frame - basic CAN frame structure
 * @can_id:  CAN ID of the frame and CAN_*_FLAG flags, see canid_t definition
 * @can_dlc: frame payload length in byte (0 .. 8) aka data length code
 *           N.B. the DLC field from ISO 11898-1 Chapter 8.4.2.3 has a 1:1
 *           mapping of the 'data length code' to the real payload length
 * @data:    CAN frame payload (up to 8 byte)
 */
struct can_frame final {
    unsigned int  can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    unsigned char rtr;/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    unsigned char can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    unsigned char data[CAN_MAX_DLEN] __attribute__((aligned(8)));
    friend std::ostream& operator<<(std::ostream& out,  can_frame value);
    can_frame& operator=(can_frame const& value) {
        can_id = value.can_id;
        rtr = value.rtr;
        can_dlc = value.can_dlc;
        can_id = value.can_id;
        memcpy(data, value.data, CAN_MAX_DLEN);
        return *this;
    };
    bool operator == (can_frame const& value) const {
        return can_id == value.can_id 
                  && rtr == value.rtr
                  && can_dlc == value.can_dlc
                  && strcmp((const char*)data, (const char*)value.data) == 0;
    };
    bool operator != (can_frame const& value) const {
        return can_id != value.can_id 
                  || rtr != value.rtr
                  || can_dlc != value.can_dlc
                  || strcmp((const char*)data, (const char*)value.data) != 0;
    };
} ;
}// namespace can
#endif // CAN_CAN_H
