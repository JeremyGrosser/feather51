#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <hidapi.h>
#include <assert.h>

#define EFM8UB1_USB_VID         0x10C4
#define EFM8UB1_USB_PID         0xEAC9

#define EFM8UB1_DEVICE_ID       0x32
#define EFM8UB1_DERIVATIVE_ID   0x41

#define EFM8_FLASH_KEYS         0xA5F1
#define EFM8_BOOTLOADER_ENABLE  0xA5
#define EFM8_BOOTLOADER_DISABLE 0x00    // THIS IS PERMANENT

/* AN945 7.1 */
enum efm8_command {
    CMD_IDENT       = 0x30,
    CMD_SETUP       = 0x31,
    CMD_ERASE       = 0x32,
    CMD_WRITE       = 0x33,
    CMD_VERIFY      = 0x34,
    CMD_LOCK        = 0x35,
    CMD_RUNAPP      = 0x36,
    CMD_VERSION     = 0xFF, // any other command returns the bootloader version
};

enum efm8_response {
    ACK             = 0x40,
    ERR_RANGE       = 0x41,
    ERR_BADID       = 0x42,
    ERR_CRC         = 0x43,
};

#define MAXLEN 64
#define CRC16_POLY 0x1021

/*
static void hexdump(uint8_t *str, size_t len) {
    for(int i = 0; i < len; i++) {
        printf("%02X ", str[i]);
    }
    printf("\n");
}
*/

static uint8_t efm8_cmd(hid_device *dev, uint8_t cmd, uint8_t *data, uint8_t len) {
    uint8_t buf[256];
    int err;
    int i;

    assert(len <= 252);

    buf[0] = '$';
    buf[1] = len;
    buf[2] = cmd;
    for(i = 0; i < len; i++) {
        buf[3+i] = data[i];
    }

    err = hid_send_feature_report(dev, buf, (len + 3));
    if(err == -1) {
        fprintf(stderr, "efm8_cmd send feature report failed: ");
        fwprintf(stderr, L"%s\n", hid_error(dev));
        return 1;
    }

    err = hid_get_feature_report(dev, buf, 1);
    if(err == -1) {
        fprintf(stderr, "efm8_cmd get feature report failed: ");
        fwprintf(stderr, L"%s\n", hid_error(dev));
        return 1;
    }

    return buf[0];
}

static const char *efm8_strerror(uint8_t err) {
    const char *msg;

    switch(err) {
        case ACK:
            msg = "ACK";
            break;
        case ERR_RANGE:
            msg = "Bad range";
            break;
        case ERR_BADID:
            msg = "Bad device ID";
            break;
        case ERR_CRC:
            msg = "CRC error";
            break;
        default:
            msg = "Unknown error";
            break;
    }

    return msg;
}

static int efm8_ident(hid_device *dev, uint8_t device_id, uint8_t derivative_id) {
    uint8_t str[2];
    int err;

    str[0] = device_id;
    str[1] = derivative_id;

    fprintf(stderr, "ident: ");
    err = efm8_cmd(dev, CMD_IDENT, str, 2);
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_setup(hid_device *dev, uint8_t bank) {
    uint8_t str[3];
    int err;

    str[0] = ((EFM8_FLASH_KEYS >> 8) & 0xFF);
    str[1] = (EFM8_FLASH_KEYS & 0xFF);
    str[2] = bank;

    fprintf(stderr, "setup: ");
    err = efm8_cmd(dev, CMD_SETUP, str, 3);
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_erase(hid_device *dev, uint16_t addr, uint8_t *data, uint8_t len) {
    uint8_t str[132];
    int err;
    int i;

    assert(len <= 128);

    str[0] = (addr >> 8) & 0xFF;
    str[1] = (addr & 0xFF);

    for(i = 0; i < len; i++) {
        str[i+2] = data[i];
    }

    fprintf(stderr, "erase: ");
    err = efm8_cmd(dev, CMD_ERASE, str, (len + 2));
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_verify(hid_device *dev, uint16_t addr1, uint16_t addr2, uint16_t crc) {
    uint8_t str[6];
    int err;

    assert(addr1 <= addr2);

    str[0] = (addr1 >> 8) & 0xFF;
    str[1] = (addr1 & 0xFF);
    str[2] = (addr2 >> 8) & 0xFF;
    str[3] = (addr2 & 0xFF);
    str[4] = (crc >> 8) & 0xFF;
    str[5] = (crc & 0xFF);

    fprintf(stderr, "verify: ");
    err = efm8_cmd(dev, CMD_VERIFY, str, 6);
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_lock(hid_device *dev, uint8_t signature, uint8_t lock) {
    uint8_t str[2];
    int err;

    str[0] = signature;
    str[1] = lock;

    fprintf(stderr, "lock: ");
    err = efm8_cmd(dev, CMD_LOCK, str, 2);
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_runapp(hid_device *dev, uint16_t option) {
    uint8_t str[2];
    int err;

    str[0] = ((option >> 8) & 0xFF);
    str[1] = (option & 0xFF);

    fprintf(stderr, "runapp: ");
    err = efm8_cmd(dev, CMD_RUNAPP, str, 2);
    if(err != ACK) {
        fprintf(stderr, "FAIL: 0x%02X %s\n", err, efm8_strerror(err));
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}

static int efm8_version(hid_device *dev) {
    uint8_t version;

    fprintf(stderr, "bootloader version: ");
    version = efm8_cmd(dev, CMD_VERSION, NULL, 0);
    switch(version) {
        case 0x90:
            fprintf(stderr, "1.0\n");
            break;
        default:
            fprintf(stderr, "unknown: 0x%02X\n", version);
    }
    return 0;
}

static uint16_t crc16(uint16_t acc, uint8_t input) {
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

int main(int argc, char *argv[]) {
    hid_device *dev;
    uint8_t *data;
    uint16_t crc = 0x0000;
    int err;
    int i;

    // open HID device
    err = hid_init();
    if(err != 0) {
        fprintf(stderr, "hid_init failed\n");
        return 1;
    }

    dev = hid_open(EFM8UB1_USB_VID, EFM8UB1_USB_PID, NULL);
    if(dev == NULL) {
        fprintf(stderr, "hid_open failed\n");
        return 1;
    }

    if(efm8_version(dev) != 0) {
        goto cleanup;
    }

    if(efm8_ident(dev, EFM8UB1_DEVICE_ID, EFM8UB1_DERIVATIVE_ID) != 0) {
        goto cleanup;
    }

    if(efm8_setup(dev, 0x00) != 0) {
        goto cleanup;
    }

    // read intel hex from stdin
    data = (uint8_t *)"\xff";
    if(efm8_erase(dev, 0x0000, data, 1) != 0) {
        goto cleanup;
    }

    for(i = 0; i < 1; i++) {
        crc = crc16(crc, data[i]);
    }

    if(efm8_verify(dev, 0x0000, 0x0000, crc) != 0) {
        goto cleanup;
    }
    
cleanup:
    hid_close(dev);
    
    err = hid_exit();
    if(err != 0) {
        fprintf(stderr, "hid_exit returned error %d\n", err);
        return 1;
    }
    return 0;
}