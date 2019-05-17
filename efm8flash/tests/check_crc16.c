#include <stdio.h>
#include <stdint.h>

#include "crc16.h"

/*
 * Test the CCITT-16 CRC function
 *
 * The CRC algorithm and test cases are defined in the EFM8UB1 Reference
 * Manual, section 14.
 */

int main(int argc, char **argv) {
    uint16_t crc;

    crc = 0xFFFF;
    crc = crc16(crc, 0x63);
    if(crc != 0xBD35) {
        printf("Testcase 14.1.1 failed\n");
        return 1;
    }

    crc = 0xFFFF;
    crc = crc16(crc, 0x8C);
    if(crc != 0xB1F4) {
        printf("Testcase 14.1.2 failed\n");
        return 2;
    }

    crc = 0xFFFF;
    crc = crc16(crc, 0x7D);
    if(crc != 0x4ECA) {
        printf("Testcase 14.1.3 failed\n");
        return 3;
    }

    crc = 0xFFFF;
    crc = crc16(crc, 0xAA);
    crc = crc16(crc, 0xBB);
    crc = crc16(crc, 0xCC);
    if(crc != 0x6CF6) {
        printf("Testcase 14.1.4 failed\n");
        return 4;
    }

    crc = 0xFFFF;
    crc = crc16(crc, 0x00);
    crc = crc16(crc, 0x00);
    crc = crc16(crc, 0xAA);
    crc = crc16(crc, 0xBB);
    crc = crc16(crc, 0xCC);
    if(crc != 0xB166) {
        printf("Testcase 14.1.5 failed\n");
        return 5;
    }

    return 0;
}
