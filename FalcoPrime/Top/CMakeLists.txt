####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
####

set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/instances.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/FalcoPrimePackets.xml"
  "${CMAKE_CURRENT_LIST_DIR}/topology.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/FalcoPrimeTopology.cpp"
)

if(NOT BUILD_TESTING)
  set(MOD_DEPS
    Falco/SystemLogger)
endif()


register_fprime_module()
