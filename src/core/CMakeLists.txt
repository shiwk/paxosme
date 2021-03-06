file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp )
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(paxosmecore
        ${HEADERS}
        ${SOURCES})

target_include_directories(paxosmecore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

