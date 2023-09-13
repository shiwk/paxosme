# set(STORE_DIR leveldb)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(storage
        ${HEADERS}
        ${SOURCES})

# target_link_libraries(storage )
target_include_directories(storage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

# libz for crc32
target_link_libraries(storage PRIVATE util leveldb glog::glog z)