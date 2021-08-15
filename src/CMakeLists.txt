include_directories(${CMAKE_CURRENT_SOURCE_DIR}/storage)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_subdirectory(core)
add_subdirectory(node)
add_subdirectory(network/grpc)

