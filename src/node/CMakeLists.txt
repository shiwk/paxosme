file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library( node
        ${SOURCES}
        ${HEADERS} )

target_link_libraries(node paxosmelib)