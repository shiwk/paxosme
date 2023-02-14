file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../*.cpp)


set(BOOSTASIO_DIR
        ""
        CACHE STRING "boost.asio include dir.")

if(BOOSTASIO_DIR STREQUAL "")
    message(SEND_ERROR "boost.asio include dir cannot be empty")
endif()


add_library(network_asio
        ${SOURCES}
        ${HEADERS} include/io_client_server.hpp)

target_link_libraries(network_asio
        paxosmecore)

target_include_directories(network_asio PRIVATE "${BOOSTASIO_DIR}" )
target_include_directories(network_asio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/ )
target_include_directories(network_asio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )