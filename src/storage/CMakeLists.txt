file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(db
        ${HEADERS}
        ${SOURCES} storage.cpp)

target_include_directories(db PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )