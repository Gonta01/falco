####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####

# Basic module dependencies
set(MOD_DEPS
  "${CMAKE_THREAD_LIBS_INIT}" 
  Fw/Cfg
  Fw/Types
  Fw/Logger
  Utils/Hash
)

# Basic source files used in every OSAL layer.  Contains common code and helpers.
set(SOURCE_FILES
# Common implementations
"${FPRIME_FRAMEWORK_PATH}/Os/Pthreads/BufferQueueCommon.cpp"
# Baremetal
"${FPRIME_FRAMEWORK_PATH}/Os/Baremetal/SystemResources.cpp"
# Custom

"${CMAKE_CURRENT_LIST_DIR}/Task.cpp"
)

register_fprime_module()

add_dependencies(Os Linux_Os)
target_link_libraries(Os PRIVATE Linux_Os)
target_link_libraries(Linux_Os PUBLIC Os)
