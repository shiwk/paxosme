# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

set(NETWORK_LIB_NAME
        ""
        CACHE STRING "network lib name")

set(NETWORK_LIB_NAME "network_grpc") # network lib
add_subdirectory(network)
add_subdirectory(util)
add_subdirectory(kvsm)
add_subdirectory(storage)
add_subdirectory(core)

add_subdirectory(node)
