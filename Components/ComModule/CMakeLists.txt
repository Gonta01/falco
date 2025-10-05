####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/ComModule.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/ComModule.cpp"
)

register_fprime_module()

set(UT_MOD_DEPS
  Fw/Test
  etl::etl
  STest
)

### UTs ###
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/ComModule.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ComModuleTester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ComModuleTesterHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/ComModuleTestMain.cpp"
)

register_fprime_ut()
