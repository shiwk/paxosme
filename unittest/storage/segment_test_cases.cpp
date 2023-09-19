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
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
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
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    const std::string &dirPath = "/tmp/segment_dir";
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {dirPath, 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitWithZeroKeyLength)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, 0};
    bool initResult = segmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitEmptyDirPath)
{
    // std::shared_ptr<LogSegmentStore> segmentStore(LogSegmentStore::New());
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {"", 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
    EXPECT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, InitWithSegmentAndReadWriteInFirstSegment)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};

    bool initResult = segmentStore->Init(logStorageOptions);
    EXPECT_TRUE(initResult);

    std::vector<SegmentIndex> segmentIndexs;

    off_t offset = 0;
    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;

    do {
        ASSERT_EQ(0, segmentId);
        const std::string &value = "test-value" + std::to_string(i);

        SegmentIndex segment_index;
        std::string key = SizeString::ToHexString(i);

        LOG(INFO) << "key:" << key << ", len:" << key.size();
        bool appendResult = segmentStore->Append(key, value, segment_index);
        LOG(INFO) << "segment_index:" << segment_index;
        segmentIndexs.push_back(segment_index);
        EXPECT_TRUE(appendResult);
        EXPECT_FALSE(segment_index.empty());

        std::string readValue;
        bool readResult = segmentStore->Read(segment_index, key, readValue);
        EXPECT_TRUE(readResult) << "key: "<< key;
        EXPECT_EQ(value, readValue);
        
        CHECKSUM checkSum;
        LogSegmentStore::ParseSegmentIndex(segment_index, segmentId, offset, checkSum);
        ++i;
    }while (offset + segmentId * segmentMaxSize < segmentMaxSize);

    for (i = segmentIndexs.size() - 1; i >= 1; --i)
    {
        const std::string &value = "test-value" + std::to_string(i);
        std::string key = SizeString::ToHexString(i);
        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndexs[i - 1], key, readValue);
        ASSERT_TRUE(readResult) << "key: "<< key;
        ASSERT_EQ(value, readValue);
    }
}

TEST_F(TestSegmentStoreTests, InitWithSegmentAndReadWriteInMultiSegments)
{
    // todo I: add test case for multi segments
}

TEST_F(TestSegmentStoreTests, InitWithSegmentExistSegment)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, SizeString::HexStringSize<instance_id_t>()};

    {
        bool initResult = segmentStore->Init(logStorageOptions);
        EXPECT_TRUE(initResult);
    }

    {
        bool initResult = segmentStore->Init(logStorageOptions);
        EXPECT_FALSE(initResult);
    }
}