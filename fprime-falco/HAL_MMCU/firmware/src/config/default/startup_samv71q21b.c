/**
 * \file
 *
 * \brief GCC startup file for ATSAMV71Q21B
 *
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

#include "definitions.h"
// #include "peripheral/mpu/plib_mpu.h"
#include "stdint.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;
extern uint32_t _psram_begin;
extern uint32_t _psram_size;
extern uint32_t _mram_begin;
extern uint32_t _mram_size;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Reset handler */
void Reset_Handler(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Disable TCM memory */
__STATIC_INLINE void __attribute__((optimize("-O1"))) TCM_Disable(void)
{
    __DSB();
    __ISB();
    SCB->ITCMCR &= ~(uint32_t)SCB_ITCMCR_EN_Msk;
    SCB->DTCMCR &= ~(uint32_t)SCB_ITCMCR_EN_Msk;
    __DSB();
    __ISB();
}
#define GPNVM_TCM_SIZE_Pos        7u
#define GPNVM_TCM_SIZE_Msk        (0x3u << GPNVM_TCM_SIZE_Pos)

/** Program GPNVM fuse for TCM configuration */
__STATIC_INLINE void __attribute__((optimize("-O1"))) TCM_Configure(uint32_t neededGpnvmValue)
{
    static uint32_t gpnvmReg;
    static uint32_t cmd;

    /* Read GPNVM fuse setting  */
    EFC_REGS->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_GGPB);
    while ((EFC_REGS->EEFC_FSR & EEFC_FSR_FRDY_Msk) == 0U)
    {
        /* Wait for FRDY bit */
    }

    gpnvmReg = EFC_REGS->EEFC_FRR;

    /* Program only if change is needed */
    if (((gpnvmReg & GPNVM_TCM_SIZE_Msk) >> GPNVM_TCM_SIZE_Pos) != neededGpnvmValue)
    {
        if((neededGpnvmValue & 0x2U) != 0U)
        {
            cmd = EEFC_FCR_FCMD_SGPB;
        }
        else
        {
            cmd = EEFC_FCR_FCMD_CGPB;
        }
        EFC_REGS->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | cmd | EEFC_FCR_FARG(8U));
        while ((EFC_REGS->EEFC_FSR & EEFC_FSR_FRDY_Msk) == 0U)
        {
            /* Wait for FRDY bit */
        }

        if((neededGpnvmValue & 0x1U) != 0U)
        {
            cmd = EEFC_FCR_FCMD_SGPB;
        }
        else
        {
            cmd = EEFC_FCR_FCMD_CGPB;
        }

        EFC_REGS->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | cmd | EEFC_FCR_FARG(7U));
        while ((EFC_REGS->EEFC_FSR & EEFC_FSR_FRDY_Msk) == 0U)
        {
            /* Wait for FRDY bit */
        }

        /* Reset the device for the programmed fuse value to take effect */
        RSTC_REGS->RSTC_CR = RSTC_CR_KEY_PASSWD | RSTC_CR_PROCRST_Msk;
    }
}

/* Enable FPU */
__STATIC_INLINE void __attribute__((optimize("-O1"))) FPU_Enable(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    SCB->CPACR |= (((uint32_t)0xFU) << 20);
    __DSB();
    __ISB();

    if (primask == 0U)
    {
        __enable_irq();
    }
}

bool memoryTest(uint8_t *baseAddr, uint32_t memorySize) {
    uint32_t i = 0;
    uint8_t *ptr = baseAddr;

    /* Clear test data area */
    for (i = 0; i < memorySize; i++) {
        ptr[i] = 0xFF;
    }

    for (i = 0; i < memorySize; i++) {
        if (i & 1) {
            ptr[i] = ((0x5a & i) & 0xff);
        } else {
            ptr[i] = ((0xa5 & i) & 0xff);
        }
    }

    for (i = 0; i < memorySize; i++) {
        if (i & 1) {
            if (ptr[i] != ((0x5a & i) & 0xff)) {
                return false;
            }
        } else {
            if (ptr[i] != ((0xa5 & i) & 0xff)) {
                return false;
            }
        }
    }
    return true;
}

#define EXTERNAL_MEMORY_TESTS_ENABLE (0)

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        /**
         * The following initialize functions are called here to initialize
         * EBI SMC peripheral for work with external memories
         */
        EFC_Initialize();
        CLOCK_Initialize();
        PIO_Initialize();
        SMC_Initialize();

#if EXTERNAL_MEMORY_TESTS_ENABLE == 1
        if (!memoryTest((uint8_t *)&_psram_begin, (uint32_t)&_psram_size)) {
            __NVIC_SystemReset();
        }
#endif

        uint32_t *pSrc, *pDest;

        /* Initialize the relocate segment */
        pSrc = &_etext;
        pDest = &_srelocate;

        if (pSrc != pDest) {
                for (; pDest < &_erelocate;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_szero; pDest < &_ezero;) {
                *pDest++ = 0;
        }

        FPU_Enable();

        TCM_Configure(0U);

        /* Disable TCM  */
        TCM_Disable();

        /* Set the vector table base address */
        pSrc = (uint32_t *) & _sfixed;
        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        /* Initialize the C library */
        __libc_init_array();

        // /* Initialize MPU */
        MPU_Initialize();

        /* Enable ICache (CMSIS-Core API) */
        SCB_EnableICache();

        /* Enable DCache (CMSIS-Core API)*/
        SCB_EnableDCache();

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}


