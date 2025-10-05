####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/IOExpander.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/IOExpander.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.

set(MOD_DEPS
    etl::etl
    Ports/IOExpanderPorts
)

register_fprime_module()

set(UT_MOD_DEPS
  Fw/Test
  STest
)

### UTs ###
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/IOExpander.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/IOExpanderTester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/IOExpanderTesterHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/IOExpanderTestMain.cpp"
)

register_fprime_ut()
