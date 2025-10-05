####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/DPS310.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/DPS310.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.

set(MOD_DEPS
    Ports/SensorPorts
    etl::etl
)

register_fprime_module()

set(UT_MOD_DEPS
  Fw/Test
  STest
)

### UTs ###
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/DPS310.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/DPS310Tester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/DPS310TesterHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/DPS310TestMain.cpp"
)

register_fprime_ut()
