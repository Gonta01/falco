file(GLOB_RECURSE root_sources ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/*.c)
file(GLOB_RECURSE config_sources ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/*.c)
file(GLOB_RECURSE packs_sources ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/*.c)
file(GLOB_RECURSE third_party_sources ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/third_party/*.c)

set(SOURCE_FILES
  ${root_sources}
  ${config_sources}
  ${packs_sources}
  ${third_party_sources}
)

register_fprime_module()

#set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
target_include_directories("${FPRIME_CURRENT_MODULE}" PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/library
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/library/tcpip
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/library/tcpip/src/common
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/default.mhc
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/peripheral
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/stdio
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/system/fs/fat_fs/file_system
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/config/default/system/fs/fat_fs/hardware_access

        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/ATSAMV71Q21B_DFP
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/ATSAMV71Q21B_DFP/component
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/ATSAMV71Q21B_DFP/instance
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/ATSAMV71Q21B_DFP/pio
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/CMSIS
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/CMSIS/CMSIS
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/CMSIS/CMSIS/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/packs/CMSIS/CMSIS/Core/Include

        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/third_party
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/third_party/rtos/FreeRTOS/Source/include
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7
)