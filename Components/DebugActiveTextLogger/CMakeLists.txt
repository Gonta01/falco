####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/DebugActiveTextLogger.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/DebugActiveTextLogger.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/LogFile.cpp"
)

register_fprime_module()

