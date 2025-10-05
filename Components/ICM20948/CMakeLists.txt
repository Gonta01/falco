####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/ICM20948.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/ICM20948.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.

set(MOD_DEPS
    etl::etl
)

register_fprime_module()

set(UT_MOD_DEPS
  Fw/Test
  Linux/Os
  STest
)

### UTs ###
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/ICM20948.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ICM20948Tester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ICM20948TesterHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ICM20948TestMain.cpp"
)

register_fprime_ut()
