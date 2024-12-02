cmake_minimum_required(VERSION 3.17)

include(${CMAKE_CURRENT_SOURCE_DIR}/examples/ecs/CMakeLists.txt)

if(NOT NEXO_BUILD_EXAMPLES)
    message(STATUS "Excluding examples from the 'ALL' target")
    set_target_properties(ecsExample PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message(STATUS "Including examples in the 'ALL' target")
endif()
