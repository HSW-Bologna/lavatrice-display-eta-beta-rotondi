#include "i2c_devices/rtc/RX8010/rx8010.h"
#include "i2c_ports/dummy/dummy_i2c_port.h"
#include "bsp/i2c_devices.h"


i2c_driver_t bsp_rtc_driver = {
    .device_address = RX8010_DEFAULT_ADDRESS,
    .i2c_transfer   = dummy_i2c_port_transfer,
};
