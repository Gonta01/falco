/**
 * \file
 *
 * \brief GCC linker script (flash) for ATSAMV71Q21B
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

/*------------------------------------------------------------------------------
 *      Linker script for running in internal FLASH on the ATSAMV71Q21B
 *----------------------------------------------------------------------------*/

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* Memory Spaces Definitions */
MEMORY
{
    rom (rx)            : ORIGIN = 0x00400000, LENGTH = 0x00200000 /* rom */
    ram (rwx)           : ORIGIN = 0x20400000, LENGTH = 0x00058000 /* ram */
    ram_nocache (rwx)   : ORIGIN = 0x20458000, LENGTH = 0x8000
    psram (rwx)         : ORIGIN = 0x61000000, LENGTH = 0x800000
    mram (rwx)          : ORIGIN = 0x62000000, LENGTH = 0x200000
}

/* The stack size used by the application. NOTE: you need to adjust according to your application. */
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x400;

/* The heapsize used by the application. NOTE: you need to adjust according to your application. */
/* WARNING. TCP IP stack example with heap_3 required heap size at least 226 960 bytes.
  This project uses heap_4, so FreeRTOS heap isn't located in system heap (system heap size is assigned here). Taking
  this into account system heap size was reduced to 153 600 bytes (initial heap size (226 960) - FreeRTOS heap size + additional
  amount of memory) */
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : DEFINED(__heap_size__) ? __heap_size__ : 0x25800;

/* The PSRAM size. */
PSRAM_SIZE = LENGTH(psram);

PROVIDE(_psram_begin = ORIGIN(psram));
PROVIDE(_psram_size = LENGTH(psram));

PROVIDE(_mram_begin = ORIGIN(mram));
PROVIDE(_mram_size = LENGTH(mram));

/* Nocached SRAM size. */
SRAM_NOCACHE_SIZE = LENGTH(ram_nocache);

/* Section Definitions */
SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > rom

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    _etext = .;

    .relocate : AT (_etext)
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        _erelocate = .;
    } > psram

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = . ;
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
        _ezero = .;
    } > mram

    /* heap section */
    .heap (NOLOAD):
    {
        . = ALIGN(8);
         _sheap = .;
        . = . + HEAP_SIZE;
        . = ALIGN(8);
        _eheap = .;
    } > ram

    /* stack section */
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack = .;
    } > ram

    .ram_nocache (NOLOAD):
    {
        . = ALIGN(4);
        _s_ram_nocache = .;
        *(.ram_nocache)
        . = ALIGN(4);
        _e_ram_nocache = .;
    } > ram_nocache

    _ram_end_ = ORIGIN(ram) + LENGTH(ram) + LENGTH(ram_nocache)  - 1 ;

    . = ORIGIN(psram);
    . = ALIGN(4);

    /* psram section */
   .psram (NOLOAD):
   {
       . = ALIGN(4);
       _spsram = .;
       *(.psram)
       *(.psram*)
       . = ALIGN(4);
       _epsram = .;
   } > psram

    . = ORIGIN(mram);
    . = ALIGN(4);

    /* mram section */
   .mram (NOLOAD):
   {
       . = ALIGN(4);
       _smram = .;
       *(.mram)
       *(.mram*)
       . = ALIGN(4);
       _emram = .;
   } > mram

    . = ALIGN(4);
    _end = .;
}
