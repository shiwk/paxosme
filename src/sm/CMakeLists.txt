file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(sm
        ${HEADERS}
        ${SOURCES})

target_include_directories(sm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )