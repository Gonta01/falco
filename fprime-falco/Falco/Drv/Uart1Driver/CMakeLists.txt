####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Uart1Driver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/Uart1Driver.cpp"
)

set(MOD_DEPS
    Ports/UartPorts
    HAL_MMCU
    etl::etl
)

register_fprime_module()
