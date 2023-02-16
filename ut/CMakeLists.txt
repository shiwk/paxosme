file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp )
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable( ut
        ${SOURCES}
        ${HEADERS} ../include/sm.hpp)

target_link_libraries(ut node)
target_link_libraries(ut network)
