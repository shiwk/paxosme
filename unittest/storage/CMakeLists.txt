enable_testing()

include_directories(${ROOT_DIR}/src/util/include)
add_subdirectory(${ROOT_DIR}/src/storage storage)
file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(test_storage ${TEST_FILES})

# set LEVELDB_BUILD_TESTS as OFF(default ON in leveldb cmakelists.txt) to build gtest separately, 
# otherwise specify option "-DLEVELDB_BUILD_TESTS=OFF".
# you cannot just link libgtest(gtest_main) built from leveldb, as the linking error of undefined symbol "typeinfo for testing::Test"   
SET(LEVELDB_BUILD_TESTS OFF CACHE BOOL "Build tests of leveldb.")

add_subdirectory(${ROOT_DIR}/3rd_party/leveldb leveldb)
target_link_libraries(test_storage gtest gtest_main)
target_link_libraries(test_storage storage)
# add_dependencies(test_storage GTest::gtest GTest::gtest_main)

add_test(NAME test_add_example COMMAND test_storage 10 24 34)