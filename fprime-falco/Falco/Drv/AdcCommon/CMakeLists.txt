####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/ADCBase.cpp"
)

set(MOD_DEPS
    HAL_MMCU
    freertos-addons
    etl::etl
)

register_fprime_module()
