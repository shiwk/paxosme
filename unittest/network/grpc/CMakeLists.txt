enable_testing()

# add_compile_definitions(NDEBUG) #disalbe debug log 


file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB TEST_ENV ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(test_network_grpc
    ${TEST_FILES}
    ${TEST_ENV}
    )

# message(STATUS "test_network_grpc: " ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(test_network_grpc PRIVATE ${ROOT_DIR}/src/network/grpc/include)
target_include_directories(test_network_grpc PRIVATE ${ROOT_DIR}/src/network/include)
target_include_directories(test_network_grpc PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/network/grpc/protogen)

target_link_libraries(test_network_grpc gtest gtest_main glog::glog network_grpc)