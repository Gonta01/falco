set(MOD_DEPS
  
)

set(SOURCE_FILES
"${CMAKE_CURRENT_LIST_DIR}/FusionAhrs.c"
"${CMAKE_CURRENT_LIST_DIR}/FusionCompass.c"
"${CMAKE_CURRENT_LIST_DIR}/FusionOffset.c"
)

register_fprime_module()

target_include_directories(Fusion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})