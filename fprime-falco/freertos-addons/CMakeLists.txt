# Basic module dependencies
set(MOD_DEPS
  HAL_MMCU
)

# Basic source files used in every OSAL layer.  Contains common code and helpers.
set(SOURCE_FILES
"${CMAKE_CURRENT_LIST_DIR}/ccondition_variable.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cevent_groups.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cmem_pool.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cmutex.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cqueue.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cread_write_lock.cpp"
"${CMAKE_CURRENT_LIST_DIR}/csemaphore.cpp"
"${CMAKE_CURRENT_LIST_DIR}/ctasklet.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cthread.cpp"
"${CMAKE_CURRENT_LIST_DIR}/ctickhook.cpp"
"${CMAKE_CURRENT_LIST_DIR}/ctimer.cpp"
"${CMAKE_CURRENT_LIST_DIR}/cworkqueue.cpp"
)

register_fprime_module()

target_include_directories("${FPRIME_CURRENT_MODULE}" PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/include
)