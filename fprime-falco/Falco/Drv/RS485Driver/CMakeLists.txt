####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/RS485Driver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/RS485Driver.cpp"
)

set(MOD_DEPS
    Ports/UartPorts
    HAL_MMCU
    )

register_fprime_module()
