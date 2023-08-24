# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/network)

add_subdirectory(util)
add_subdirectory(kvsm)
add_subdirectory(storage)
add_subdirectory(core)

add_subdirectory(network)
add_subdirectory(node)
