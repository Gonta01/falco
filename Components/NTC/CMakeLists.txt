####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/NTC.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/NTC.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.

set(MOD_DEPS
  etl::etl
  Ports/AdcPorts
)

register_fprime_module()

set(UT_MOD_DEPS
  Fw/Test
  STest
)

set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/NTC.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/NTCTestMain.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/NTCTester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/NTCTesterHelpers.cpp"
)

register_fprime_ut()
