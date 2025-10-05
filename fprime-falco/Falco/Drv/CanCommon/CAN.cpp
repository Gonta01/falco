#include <etl/array.h>
#include "mcan/plib_mcan0.h"
#include "mcan/plib_mcan1.h"
#include "mcan/plib_mcan_common.h"

namespace Base {
    etl::array<uint8_t, MCAN0_MESSAGE_RAM_CONFIG_SIZE> MCAN0MessageRAM __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));
    etl::array<uint8_t, MCAN1_MESSAGE_RAM_CONFIG_SIZE> MCAN1MessageRAM __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));
}