file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp )
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable( ft
        ${SOURCES}
        ${HEADERS})

target_link_libraries(ft node)
target_link_libraries(ft network)
target_link_libraries(ft sm)
