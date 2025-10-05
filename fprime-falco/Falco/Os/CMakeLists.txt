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
  HAL_MMCU
  etl::etl
  Fw/Cfg
  Fw/Types
  Fw/Logger
  Utils/Hash
  HAL_MMCU
  etl::etl
)

# Basic source files used in every OSAL layer.  Contains common code and helpers.
set(SOURCE_FILES
# Common implementations
"${FPRIME_FRAMEWORK_PATH}/Os/Pthreads/BufferQueueCommon.cpp"
# Baremetal
"${FPRIME_FRAMEWORK_PATH}/Os/Baremetal/SystemResources.cpp"
# Custom
"${CMAKE_CURRENT_LIST_DIR}/InterruptLock.cpp"
"${CMAKE_CURRENT_LIST_DIR}/IntervalTimer.cpp"
"${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp"
"${CMAKE_CURRENT_LIST_DIR}/Queue.cpp"
"${CMAKE_CURRENT_LIST_DIR}/Task.cpp"
"${CMAKE_CURRENT_LIST_DIR}/TaskId.cpp"
"${CMAKE_CURRENT_LIST_DIR}/CommonFileSystem.cpp"
"${CMAKE_CURRENT_LIST_DIR}/File.cpp"
"${CMAKE_CURRENT_LIST_DIR}/FileSystem.cpp"
"${CMAKE_CURRENT_LIST_DIR}/Directory.cpp"
)

register_fprime_module()

add_dependencies(Os Falco_Os)
target_link_libraries(Os PRIVATE Falco_Os)
target_link_libraries(Falco_Os PUBLIC Os)