#include <gtest/gtest.h>
#include <segment_store.hpp>
#include "segment_test_helper.hpp"

// TEST_F(TestAddIntExample, test_add_int)
// {
//     int res = 10 + 24;
//     EXPECT_EQ(res, 34);
// }

TEST_F(TestSegmentStoreTests, SegmentStoreInit)
{
    // std::shared_ptr<LogSegmentStore> semgmentStore(LogSegmentStore::New());
    auto semgmentStore = LongLife::CreateInstance<LogSegmentStore>();
    
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_TRUE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitDirPathNotExists)
{
    // std::shared_ptr<LogSegmentStore> semgmentStore(LogSegmentStore::New());
    auto semgmentStore = LongLife::CreateInstance<LogSegmentStore>();
    const std::string &dirPath = "/tmp/segment_dir";
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {dirPath, 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitEmptyDirPath)
{
    // std::shared_ptr<LogSegmentStore> semgmentStore(LogSegmentStore::New());
    auto semgmentStore = LongLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {"", 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}