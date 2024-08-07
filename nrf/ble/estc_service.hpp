#pragma once

#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#include "hsv.hpp"

// TODO: 1. Generate random BLE UUID (Version 4 UUID) and define it in the following format:
#define ESTC_BASE_UUID { 0x89, 0xDF, 0x39, 0x6B, 0xDE, 0x58, /* - */ 0x3D, 0x80, /* - */ 0x05, 0x48, /* - */ 0x81, 0x43, /* - */ 0xB9, 0x51, 0x70, 0x3C } // UUID: 3C7051B9-4381-4805-803D-58DE6B39DF89

// TODO: 2. Pick a random service 16-bit UUID and define it:
#define ESTC_SERVICE_UUID 0x51B9

// TODO: 3. Pick a characteristic UUID and define it:

#define ESTC_GATT_CHAR_WRITE_UUID  0xDC01
#define ESTC_GATT_CHAR_NOTIFY_UUID 0xDC02

typedef struct
{
    uint16_t service_handle;
    uint16_t connection_handle;

    // TODO: 6.3. Add handles for characterstic (type: ble_gatts_char_handles_t)
    ble_gatts_char_handles_t characteristic_write_handle;
    ble_gatts_char_handles_t characteristic_notify_handle;
} ble_estc_service_t;

ret_code_t estc_ble_service_init(ble_estc_service_t *service);

ret_code_t estc_ble_send_characteristic_value(uint16_t conn_handle, uint16_t value_handle, uint8_t type, uint16_t len, const uint8_t* value);

#endif /* ESTC_SERVICE_H__ */