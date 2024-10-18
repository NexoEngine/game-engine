cmake_minimum_required(VERSION 3.17)

# Include FetchContent module
include(FetchContent)

# Declare reflect-cpp dependency
FetchContent_Declare(
        reflectcpp
        GIT_REPOSITORY https://github.com/getml/reflect-cpp.git
        GIT_TAG main
)

set(REFLECTCPP_XML OFF) # Enable XML support
set(REFLECTCPP_FLEXBUFFERS OFF) # Enable FlexBuffers support

# Fetch the content at configure time
FetchContent_MakeAvailable(reflectcpp)
