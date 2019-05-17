#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>

#define CRC16_POLY     0x1021

uint16_t crc16(uint16_t acc, uint8_t input) {
    int i;
    acc = acc ^ (input << 8);

    for(i = 0; i < 8; i++) {
        if((acc & 0x8000) == 0x8000) {
            acc = acc << 1;
            acc ^= CRC16_POLY;
        }else{
            acc = acc << 1;
        }
    }

    return acc;
}

#endif
