include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_subdirectory(sm)
add_subdirectory(storage)
add_subdirectory(core)

add_subdirectory(node)
add_subdirectory(network/grpc)
