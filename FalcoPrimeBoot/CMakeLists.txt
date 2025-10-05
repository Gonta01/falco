#####
# 'FalcoPrime' Deployment:
#
# This registers the 'FalcoPrime' deployment to the build system. 
# Custom components that have not been added at the project-level should be added to 
# the list below.
#
#####

###
# Topology and Components
###

set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
                 "${CMAKE_CURRENT_LIST_DIR}/ExternalFlash.cpp")

register_fprime_deployment()

if(NOT BUILD_TESTING)
    finalize_sbc_executable()
endif()
