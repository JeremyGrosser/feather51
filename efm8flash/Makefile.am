bin_PROGRAMS=efm8flash
efm8flash_CFLAGS=$(HIDAPI_CFLAGS) -Wall -std=c99
efm8flash_LDADD=$(HIDAPI_LIBS)
efm8flash_SOURCES=efm8flash.c

check_PROGRAMS=check_crc16
check_crc16_SOURCES=tests/check_crc16.c
TESTS=check_crc16
