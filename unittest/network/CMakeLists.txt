enable_testing()

set(NETWORK_LIB_NAME
        ""
        CACHE STRING "network lib name")

# add_definitions(-DFINITE_SCHEDULE_LOOP)

set(NETWORK_LIB_NAME "network_grpc") # network lib

add_subdirectory(${ROOT_DIR}/src/network network)

add_subdirectory(grpc)
add_subdirectory(asio)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB TEST_ENV ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(test_network
    ${TEST_FILES}
    ${TEST_ENV}
    )

target_link_libraries(test_network gtest gtest_main glog::glog network)