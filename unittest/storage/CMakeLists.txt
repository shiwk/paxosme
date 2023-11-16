enable_testing()

add_definitions(-DFINITE_SCHEDULE_LOOP)
add_subdirectory(${ROOT_DIR}/src/util util)
add_subdirectory(${ROOT_DIR}/src/storage storage)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB TEST_ENV ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(test_storage 
    ${TEST_FILES}
    ${TEST_ENV}
    )

# set LEVELDB_BUILD_TESTS as OFF(default ON in leveldb cmakelists.txt) to build gtest separately, 
# otherwise specify option "-DLEVELDB_BUILD_TESTS=OFF".
# you cannot just link libgtest(gtest_main) built from leveldb, as the linking error of undefined symbol "typeinfo for testing::Test"   
SET(LEVELDB_BUILD_TESTS OFF CACHE BOOL "Build tests of leveldb.")


add_subdirectory(${ROOT_DIR}/3rd_party/leveldb leveldb)
target_link_libraries(test_storage gtest gtest_main)

target_link_libraries(test_storage storage leveldb util glog::glog)
# add_dependencies(test_storage GTest::gtest GTest::gtest_main)

# target_include_directories(test_storage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# add_test(NAME test_add_example COMMAND test_storage 10 24 34)