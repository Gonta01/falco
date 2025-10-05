####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
include(utilities)
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriver.cpp"
)

set(MOD_DEPS
    HAL_MMCU
)

register_fprime_module()

