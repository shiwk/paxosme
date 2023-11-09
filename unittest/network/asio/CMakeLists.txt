enable_testing()

# add_compile_definitions(NDEBUG) #disalbe debug log 
# add_subdirectory(${ROOT_DIR}/src/network network)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB TEST_ENV ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(test_network_asio
    ${TEST_FILES}
    ${TEST_ENV}
    )


target_include_directories(test_network_asio PRIVATE ${ROOT_DIR}/src/network/asio/include)
target_include_directories(test_network_asio PRIVATE ${ROOT_DIR}/src/network/include)

target_link_libraries(test_network_asio gtest gtest_main glog::glog network_asio)