####
# This sets up the build system for the 'FPrime_SBC' project, including
# components and deployments from project.cmake. In addition, it imports the core F Prime components.
####

cmake_minimum_required(VERSION 3.30)
project(FPrime_SBC C CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++17> $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-I${CMAKE_CURRENT_SOURCE_DIR}/ETLConfig>)

if (ENABLE_ERROR_LOGGING_IN_FILE STREQUAL "ON")
    add_compile_definitions(ERROR_LOGGING_IN_FILE)
endif()

###
# F' Core Setup
# This includes all of the F prime core components, and imports the make-system.
###
include("${CMAKE_CURRENT_LIST_DIR}/fprime/cmake/FPrime.cmake")
Include(FetchContent)

FetchContent_Declare(
        etl
        GIT_REPOSITORY https://github.com/ETLCPP/etl
        GIT_TAG        20.37.2
)
FetchContent_MakeAvailable(etl)
# NOTE: register custom targets between these two lines
fprime_setup_included_code()


# This includes project-wide objects
include("${CMAKE_CURRENT_LIST_DIR}/project.cmake")
