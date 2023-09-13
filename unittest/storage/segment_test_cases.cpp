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
    auto semgmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_TRUE(initResult);

    std::string dbPath = DirPath + "/" + SEGMENT_STORE_DIR;
    bool dirExists = TestSegmentStoreTests::DirExists(dbPath);
    EXPECT_TRUE(dirExists);

    std::string metaPath = dbPath + "/" + METADATA_FILE;
    dirExists = TestSegmentStoreTests::DirExists(metaPath);
    EXPECT_TRUE(dirExists);

    std::string segmentPath = dbPath + "/0";
    dirExists = TestSegmentStoreTests::DirExists(segmentPath);
    EXPECT_TRUE(dirExists);

    size_t segmentSize = TestSegmentStoreTests::FileSize(segmentPath);
    EXPECT_EQ(1024, segmentSize);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitDirPathNotExists)
{
    auto semgmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    const std::string &dirPath = "/tmp/segment_dir";
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {dirPath, 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitEmptyDirPath)
{
    // std::shared_ptr<LogSegmentStore> semgmentStore(LogSegmentStore::New());
    auto semgmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {"", 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}


TEST_F(TestSegmentStoreTests, InitWithSegmentAndReadWrite)
{
    auto semgmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024};
    
    {
        bool initResult = semgmentStore->Init(logStorageOptions);
        EXPECT_TRUE(initResult);
    }

    {
        bool initResult = semgmentStore->Init(logStorageOptions);
        EXPECT_FALSE(initResult);
    }

}


TEST_F(TestSegmentStoreTests, InitWithSegmentExistSegment)
{
    auto semgmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024};
    
    {
        bool initResult = semgmentStore->Init(logStorageOptions);
        EXPECT_TRUE(initResult);
    }

    {
        bool initResult = semgmentStore->Init(logStorageOptions);
        EXPECT_FALSE(initResult);
    }

}