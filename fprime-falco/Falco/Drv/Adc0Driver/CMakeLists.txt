####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Adc0Driver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adc0Driver.cpp"
)

set(MOD_DEPS
    HAL_MMCU
    Ports/AdcPorts
    Falco/Drv/AdcCommon
)

register_fprime_module()
