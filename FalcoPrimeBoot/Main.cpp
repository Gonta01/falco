// ======================================================================
// \title  Main.cpp
// \brief main program for the F' application.
//
// ======================================================================

#include "definitions.h"
#include <cstdint>
#include "ExternalFlash.hpp"

#include "peripheral/mpu/plib_mpu.h"
#include "peripheral/mpu/plib_mpu_local.h"

uint32_t __start_sp;
uint32_t (*__start_new)(void);

extern "C" {
void app_init() {

    // 1. Flash chip init
    bool result = externalFlashInit();

    if (result == false) {
        __NVIC_SystemReset();
    }

    // 2. Read stack pointer and reset handler pointer
    qspi_memory_xfer_t nativeConfig;
    nativeConfig.instruction = fastReadQuadIO4byteCode;
    nativeConfig.option = 0;
    nativeConfig.width = QUAD_IO;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.option_en = true;
    nativeConfig.option_len = OPTL_8_BIT;
    nativeConfig.continuous_read_en = false; // continuous read enable
    nativeConfig.dummy_cycles = 4;

    uint32_t stackPointer = 0;
    uint32_t stackPointerMemAddr = 0;

    result = QSPI_MemoryRead(&nativeConfig, &stackPointer, sizeof(stackPointer), stackPointerMemAddr);
    if (result == false) {
        __NVIC_SystemReset();
    }

    uint32_t resetHandlerFunction = 0;
    uint32_t resetHandlerMemAddr = 4;
    result = QSPI_MemoryRead(&nativeConfig, &resetHandlerFunction, sizeof(resetHandlerFunction), resetHandlerMemAddr);
    if (result == false) {
        __NVIC_SystemReset();
    }

    // 3. Activate continuous read mode
    result = exitQPIMode();
    if (result == false) {
        __NVIC_SystemReset();
    }

    /*** Disable MPU            ***/
    MPU->CTRL = 0;

    /* Region 4 Name: QSPI, Base Address: 0x80000000, Size: 512MB  */
    MPU->RBAR = MPU_REGION(4U, 0x80000000U);
    MPU->RASR = MPU_REGION_SIZE(28U) | MPU_RASR_AP(MPU_RASR_AP_READWRITE_Val) | MPU_ATTR_NORMAL_WB_WA \
                | MPU_ATTR_ENABLE  | MPU_ATTR_SHAREABLE;

    /* Enable Memory Management Fault */
    SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk);

    /* Enable MPU */
    MPU->CTRL = MPU_CTRL_ENABLE_Msk  | MPU_CTRL_PRIVDEFENA_Msk;

    __DSB();
    __ISB();

    result = enableContinuousRead();
    if (result == false) {
        __NVIC_SystemReset();
    }

    // 4. Final configurations
    /* Set PC and SP of the XIP Image application programmed in QSPI Flash memory*/
    /* Pointer to Reset Handler of the XIP Image Application */
    __start_new = (uint32_t(*) (void)) resetHandlerFunction;  // For testing only
    __start_sp = stackPointer;                        // For testing only

    __set_MSP(__start_sp);

    /* Rebase the vector table base address */
    SCB->VTOR = ((uint32_t) QSPIMEM_ADDR & SCB_VTOR_TBLOFF_Msk);

    /* Run XIP Image Application */
    __start_new();

}
}