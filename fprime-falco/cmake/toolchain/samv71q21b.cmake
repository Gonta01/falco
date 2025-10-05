set(CMAKE_SYSTEM_NAME FreeRTOS)
set(FPRIME_PLATFORM "Falco")
# Specify that a thread package should be searched in the toolchain directory

# Specify the cross compiler
set(ARM_TOOLCHAIN /opt/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/bin)
set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN}/arm-none-eabi-g++)

# Path to root of toolchain package
set(CMAKE_FIND_ROOT_PATH ARM_TOOLCHAIN)

# DO NOT EDIT
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Toolchain Specific
set(BUILD_FW_DP OFF CACHE BOOL "Build Fw/Dp components")

set(LINK_FLAGS "--specs=nosys.specs --specs=nano.specs -lc_nano -mcpu=cortex-m7 -gdwarf-2 -D__SAMV71Q21B__ -mthumb -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--print-memory-usage -Wl,-e exception_table")

if (APP_MODE STREQUAL "BOOT")
    set(LINK_FLAGS "${LINK_FLAGS} -T${CMAKE_SOURCE_DIR}/fprime-falco/HAL_MMCU_BOOT/firmware/src/config/default/samv71q21b_boot.ld")
elseif(APP_MODE STREQUAL "APP_XIP")
    set(LINK_FLAGS "${LINK_FLAGS} -T${CMAKE_SOURCE_DIR}/fprime-falco/HAL_MMCU/firmware/src/config/default/samv71q21b_app_xip.ld")
elseif(APP_MODE STREQUAL "APP_STANDARD")
    set(LINK_FLAGS "${LINK_FLAGS} -T${CMAKE_SOURCE_DIR}/fprime-falco/HAL_MMCU/firmware/src/config/default/samv71q21b_app_extram.ld")
endif()

set(C_COMPILE_FLAGS "-Werror=unused-result -ffreestanding -mcpu=cortex-m7 -fno-builtin -g -D__DEBUG -D__SAMV71Q21B__ -DHAVE_CONFIG_H -DWOLFSSL_IGNORE_FILE_WARN -gdwarf-2  -ffunction-sections -mlong-calls -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -MP -MMD -MF")
set(CXX_COMPILE_FLAGS ${C_COMPILE_FLAGS}" -fno-rtti -fno-exceptions")

# # Prevent CMake from adding its own -mcpu and -mthumb flags which conflict with the specific flags required for the ATSAME70Q21B processor
set(CMAKE_C_FLAGS "${C_COMPILE_FLAGS}" CACHE STRING "Initial C FLAGS" FORCE)
set(CMAKE_CXX_FLAGS "${CXX_COMPILE_FLAGS}" CACHE STRING "Initial CXX FLAGS" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${LINK_FLAGS}" CACHE STRING "Initial EXE LINKER FLAGS" FORCE)

# Remove POSIX
set(FPRIME_USE_POSIX OFF)

# Verbose flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -v")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -v")

include("${CMAKE_CURRENT_LIST_DIR}/support/samv71q21b-support.cmake")