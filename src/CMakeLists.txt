include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_subdirectory(util)
add_subdirectory(sm)
add_subdirectory(storage)
add_subdirectory(core)

add_subdirectory(network/grpc)
add_subdirectory(node)
