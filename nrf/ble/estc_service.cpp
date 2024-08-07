#include "estc_service.hpp"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

#include "hsv.hpp"

static ret_code_t estc_ble_add_write_characteristics(ble_estc_service_t* service);
static ret_code_t estc_ble_add_notify_characteristics(ble_estc_service_t* service);

ret_code_t estc_ble_service_init(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    // TODO: 4. Add service UUIDs to the BLE stack table using `sd_ble_uuid_vs_add`
    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
    ble_uuid_t service_uuid = {ESTC_SERVICE_UUID, BLE_UUID_TYPE_BLE};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(error_code);

    // TODO: 5. Add service to the BLE stack using `sd_ble_gatts_service_add`
    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &service->service_handle);
    APP_ERROR_CHECK(error_code);

    // NRF_LOG_DEBUG("%s:%d | Service UUID: 0x%04x", __FUNCTION__, __LINE__, service_uuid.uuid);
    // NRF_LOG_DEBUG("%s:%d | Service UUID type: 0x%02x", __FUNCTION__, __LINE__, service_uuid.type);
    // NRF_LOG_DEBUG("%s:%d | Service handle: 0x%04x", __FUNCTION__, __LINE__, service->service_handle);

    error_code = estc_ble_add_write_characteristics(service);
    APP_ERROR_CHECK(error_code);
    error_code = estc_ble_add_notify_characteristics(service);
    APP_ERROR_CHECK(error_code);

    return error_code;
}

static ret_code_t estc_ble_add_write_characteristics(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    // TODO: 6.1. Add custom characteristic UUID using `sd_ble_uuid_vs_add`, same as in step 4
    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
    ble_uuid_t characteristic_uuid = {ESTC_GATT_CHAR_WRITE_UUID, BLE_UUID_TYPE_BLE};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &characteristic_uuid.type);
    APP_ERROR_CHECK(error_code);

    // TODO: 6.5. Configure Characteristic metadata (enable read and write)
    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    // Configures attribute metadata. For now we only specify that the attribute will be stored in the softdevice
    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    // TODO: 6.6. Set read/write security levels to our attribute metadata using `BLE_GAP_CONN_SEC_MODE_SET_OPEN`
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    // TODO: 6.2. Configure the characteristic value attribute (set the UUID and metadata)
    ble_gatts_attr_t attr_char_value = { 0 };
    attr_char_value.p_uuid = &characteristic_uuid;
    attr_char_value.p_attr_md = &attr_md;

    // TODO: 6.7. Set characteristic length in number of bytes in attr_char_value structure
    attr_char_value.init_len = sizeof(nrf::hsv);
    attr_char_value.max_len = sizeof(nrf::hsv);

    // TODO: 6.4. Add new characteristic to the service using `sd_ble_gatts_characteristic_add`
    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &service->characteristic_write_handle);
    APP_ERROR_CHECK(error_code);

    return error_code;
}

static ret_code_t estc_ble_add_notify_characteristics(ble_estc_service_t* service)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid128_t base_uuid = { ESTC_BASE_UUID };
    ble_uuid_t characteristic_uuid = { ESTC_GATT_CHAR_NOTIFY_UUID, BLE_UUID_TYPE_BLE };

    error_code = sd_ble_uuid_vs_add(&base_uuid, &characteristic_uuid.type);
    APP_ERROR_CHECK(error_code);

    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read = 1;
    char_md.char_props.notify = 1;

    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t attr_char_value = { 0 };
    attr_char_value.p_uuid = &characteristic_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof(nrf::hsv);
    attr_char_value.max_len = sizeof(nrf::hsv);

    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &service->characteristic_notify_handle);
    APP_ERROR_CHECK(error_code);

    return error_code;
}

ret_code_t estc_ble_send_characteristic_value(uint16_t conn_handle, uint16_t value_handle, uint8_t type, uint16_t len, const uint8_t* value)
{
    NRF_LOG_INFO("Sending value");

    ret_code_t error_code = NRF_SUCCESS;

    ble_gatts_hvx_params_t hvx_params = { 0 };

    hvx_params.handle = value_handle;
    hvx_params.type = type;
    hvx_params.offset = 0,
    hvx_params.p_len = &len;
    hvx_params.p_data = value;

    error_code = sd_ble_gatts_hvx(conn_handle, &hvx_params);

    return error_code;
}

