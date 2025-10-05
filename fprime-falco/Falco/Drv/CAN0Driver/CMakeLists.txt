####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/CAN0Driver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/CAN0Driver.cpp"
)

set(MOD_DEPS
    Ports/CanPorts
    Falco/Drv/CanCommon
    HAL_MMCU
    etl::etl
)

register_fprime_module()
