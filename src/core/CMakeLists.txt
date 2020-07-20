file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(paxosmelib
        ${HEADERS}
        ${SOURCES})

target_include_directories(paxosmelib PUBLIC ../types)
