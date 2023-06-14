file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp )
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


set(BOOST_ROOT_DIR
        ""
        CACHE STRING "boost.asio include dir.")

if(BOOST_ROOT_DIR STREQUAL "")
    message(SEND_ERROR "boost.asio include dir cannot be empty")
endif()


add_library(network_asio
        ${SOURCES}
        ${HEADERS})

#target_link_libraries(network_asio
#        paxosmecore)

target_include_directories(network_asio PRIVATE ${BOOST_ROOT_DIR} )
target_include_directories(network_asio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/ )
target_include_directories(network_asio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )