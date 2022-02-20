file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(util
        ${HEADERS}
        ${SOURCES})

target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )