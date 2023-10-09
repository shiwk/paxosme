#include <gtest/gtest.h>
#include <glog/logging.h>
#include <segment_store.hpp>
#include "segment_test_helper.hpp"

// TEST_F(TestAddIntExample, test_add_int)
// {
//     int res = 10 + 24;
//     ASSERT_EQ(res, 34);
// }

TEST_F(TestSegmentStoreTests, SegmentStoreInit)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    std::string dbPath = DirPath + "/" + SEGMENT_STORE_DIR;
    bool dirExists = TestSegmentStoreTests::DirExists(dbPath);
    ASSERT_TRUE(dirExists);

    std::string metaPath = dbPath + "/" + METADATA_FILE;
    dirExists = TestSegmentStoreTests::DirExists(metaPath);
    ASSERT_TRUE(dirExists);

    std::string segmentPath = dbPath + "/0";
    dirExists = TestSegmentStoreTests::DirExists(segmentPath);
    ASSERT_TRUE(dirExists);

    size_t segmentSize = TestSegmentStoreTests::FileSize(segmentPath);
    ASSERT_EQ(1024, segmentSize);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitDirPathNotExists)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    const std::string &dirPath = "/tmp/segment_dir";
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {dirPath, 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitWithZeroKeyLength)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, 0};
    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, SegmentStoreInitEmptyDirPath)
{
    // std::shared_ptr<LogSegmentStore> segmentStore(LogSegmentStore::New());
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {"", 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_FALSE(initResult);
}

TEST_F(TestSegmentStoreTests, InitWithSegmentAndReadWriteInFirstSegment)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};

    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    std::vector<SegmentIndex> segmentIndexs;

    off_t offset = 0;
    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;

    do
    {
        ASSERT_EQ(0, segmentId);
        const std::string &value = "test-value" + std::to_string(i);

        SegmentIndex segmentIndex;
        std::string key = SizeString::ToHexString(i);

        // LOG(INFO) << "key:" << key << ", len:" << key.size();
        bool appendResult = segmentStore->Append(key, value, segmentIndex);
        // LOG(INFO) << "segmentIndex:" << segmentIndex;
        segmentIndexs.push_back(segmentIndex);
        ASSERT_TRUE(appendResult);
        ASSERT_FALSE(segmentIndex.empty());

        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndex, key, readValue);
        ASSERT_TRUE(readResult) << "key: " << key;
        ASSERT_EQ(value, readValue);

        CHECKSUM checkSum;
        LogSegmentStore::ParseSegmentIndex(segmentIndex, segmentId, offset, checkSum);
        ++i;
    } while (offset + segmentId * segmentMaxSize < segmentMaxSize);

    for (i = segmentIndexs.size() - 1; i >= 1; --i)
    {
        const std::string &value = "test-value" + std::to_string(i);
        std::string key = SizeString::ToHexString(i);
        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndexs[i - 1], key, readValue);
        ASSERT_TRUE(readResult) << "key: " << key;
        ASSERT_EQ(value, readValue);
    }
}

TEST_F(TestSegmentStoreTests, ReadWriteInMultiSegments)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};

    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    std::vector<SegmentIndex> segmentIndexs;

    off_t offset = 0;
    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;

    do
    {
        ASSERT_EQ(0, segmentId);
        const std::string &value = "test-value" + std::to_string(i);

        SegmentIndex segmentIndex;
        std::string key = SizeString::ToHexString(i);

        // LOG(INFO) << "key:" << key << ", len:" << key.size();
        bool appendResult = segmentStore->Append(key, value, segmentIndex);
        // LOG(INFO) << "segmentIndex:" << segmentIndex;
        segmentIndexs.push_back(segmentIndex);
        ASSERT_TRUE(appendResult);
        ASSERT_FALSE(segmentIndex.empty());

        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndex, key, readValue);
        // ASSERT_TRUE(readResult) << "key: "<< key;
        ASSERT_EQ(value, readValue);

        CHECKSUM checkSum;
        LogSegmentStore::ParseSegmentIndex(segmentIndex, segmentId, offset, checkSum);
        ++i;
    } while (offset + segmentId * segmentMaxSize < segmentMaxSize);

    const std::string &value = "test-value" + std::to_string(i);

    SegmentIndex segmentIndex;
    std::string key = SizeString::ToHexString(i);

    bool appendResult = segmentStore->Append(key, value, segmentIndex);
    ASSERT_TRUE(appendResult);
    ASSERT_FALSE(segmentIndex.empty());

    std::string readValue;
    bool readResult = segmentStore->Read(segmentIndex, key, readValue);
    ASSERT_EQ(value, readValue);

    CHECKSUM checkSum;
    LogSegmentStore::ParseSegmentIndex(segmentIndex, segmentId, offset, checkSum);
    ASSERT_EQ(1, segmentId);
}

TEST_F(TestSegmentStoreTests, InitWithSegmentExistSegment)
{
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, SizeString::HexStringSize<instance_id_t>()};

    auto segmentStore1 = ShortLife::CreateInstance<LogSegmentStore>();
    bool initResult = segmentStore1->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    SegmentIndex segmentIndex;
    instance_id_t i = 1;
    std::string key = SizeString::ToHexString(i);
    const std::string &value = "test-value" + std::to_string(i);
    bool appendResult = segmentStore1->Append(key, value, segmentIndex);
    ASSERT_TRUE(appendResult);

    std::string readValue;
    bool readResult = segmentStore1->Read(segmentIndex, key, readValue);
    ASSERT_TRUE(readResult);
    ASSERT_EQ(value, readValue);

    auto segmentStore2 = ShortLife::CreateInstance<LogSegmentStore>();
    initResult = segmentStore2->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    readResult = segmentStore2->Read(segmentIndex, key, readValue);
    ASSERT_TRUE(readResult);
    ASSERT_EQ(value, readValue);
}

TEST_F(TestSegmentStoreTests, ReadWriteMultiSize)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};

    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    std::vector<SegmentIndex> segmentIndexs;

    off_t offset = 0;
    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;

    do
    {
        const std::string &value = std::string(i, 'a');
        SegmentIndex segmentIndex;
        std::string key = SizeString::ToHexString(i);

        bool appendResult = segmentStore->Append(key, value, segmentIndex);
        segmentIndexs.push_back(segmentIndex);
        ASSERT_TRUE(appendResult);
        ASSERT_FALSE(segmentIndex.empty());

        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndex, key, readValue);
        ASSERT_TRUE(readResult) << "key: " << key << " i: " << i;
        ASSERT_EQ(value, readValue);
    } while (sizeof(size_t) + SizeString::HexStringSize<instance_id_t>() + ++i <= segmentMaxSize);

    for (i = 1; i <= segmentIndexs.size(); ++i)
    {
        std::string key = SizeString::ToHexString(i);
        std::string readValue;
        bool readResult = segmentStore->Read(segmentIndexs[i - 1], key, readValue);
        ASSERT_TRUE(readResult) << "key: " << key;
        const std::string &value = std::string(i, 'a');
        ASSERT_EQ(value, readValue);
    }
}

TEST_F(TestSegmentStoreTests, TestReplayLog)
{
    auto segmentStore1 = ShortLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, 1024, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore1->Init(logStorageOptions);

    SegmentIndex segmentIndex;
    instance_id_t i = 1;
    std::string key = SizeString::ToHexString(i);
    const std::string &value = "test-value" + std::to_string(i);
    bool appendResult = segmentStore1->Append(key, value, segmentIndex);
    std::string readValue;
    bool readResult = segmentStore1->Read(segmentIndex, key, readValue);

    auto segmentStore2 = ShortLife::CreateInstance<LogSegmentStore>();
    segmentStore2->Init(logStorageOptions);
    SegmentIndex replayIndex;
    std::string replayKey;
    off_t offset = 0;
    int repalyRes = segmentStore2->ReplayLog(0, offset, replayKey, replayIndex);
    size_t pos = sizeof(size_t) + SizeString::HexStringSize<instance_id_t>() + value.size();
    ASSERT_EQ(pos, repalyRes);
    ASSERT_EQ(key, replayKey);
}

TEST_F(TestSegmentStoreTests, TestReplayMultiLogs)
{
    auto segmentStore1 = ShortLife::CreateInstance<LogSegmentStore>();
    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};
    bool initResult = segmentStore1->Init(logStorageOptions);

    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;
    do
    {
        const std::string &value = std::string(i, 'a');
        SegmentIndex segmentIndex;
        std::string key = SizeString::ToHexString(i);

        bool appendResult = segmentStore1->Append(key, value, segmentIndex);
        ASSERT_TRUE(appendResult);
        ASSERT_FALSE(segmentIndex.empty());
    } while (sizeof(size_t) + SizeString::HexStringSize<instance_id_t>() + ++i <= segmentMaxSize);
    instance_id_t maxInstanceId = i - 1;
    LOG(INFO) << "maxInstanceId: " << maxInstanceId;

    auto segmentStore2 = ShortLife::CreateInstance<LogSegmentStore>();
    segmentStore2->Init(logStorageOptions);

    segmentId = 0;
    SegmentIndex replayIndex;
    std::string replayKey;
    off_t offset = 0;
    i = 1;
    while (i <= maxInstanceId)
    {
        std::string key = SizeString::ToHexString(i);
        const std::string &value = std::string(i, 'a');
        int repalyOffset = segmentStore2->ReplayLog(segmentId, offset, replayKey, replayIndex);
        if (repalyOffset == 0)
        {
            segmentId++;
            offset = 0;
            continue;
        }

        off_t tmpOffset = offset + sizeof(size_t) + SizeString::HexStringSize<instance_id_t>() + value.size();
        ASSERT_EQ(tmpOffset, repalyOffset) << "offset: " << offset << ", segmentId: " << segmentId << ", i: " << i << ", repalyKey: " << replayKey << ", key: " << key;
        ASSERT_EQ(key, replayKey);
        offset = repalyOffset;
        ++i;
    }
}

TEST_F(TestSegmentStoreTests, TestRemoveAsync)
{
    auto segmentStore = ShortLife::CreateInstance<LogSegmentStore>();

    size_t segmentMaxSize = 1024;
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {DirPath, segmentMaxSize, SizeString::HexStringSize<instance_id_t>()};

    bool initResult = segmentStore->Init(logStorageOptions);
    ASSERT_TRUE(initResult);

    std::vector<SegmentIndex> segmentIndexs;

    off_t offset = 0;
    instance_id_t i = 1;
    SEGMENT_ID segmentId = 0;
    size_t segment_bytes = 0;

    while (segment_bytes < segmentMaxSize)
    {
        const std::string &value = std::string(i, 'a');
        SegmentIndex segmentIndex;
        std::string key = SizeString::ToHexString(i++);


        bool appendResult = segmentStore->Append(key, value, segmentIndex);
        segment_bytes += sizeof(size_t) + SizeString::HexStringSize<instance_id_t>() + value.size();

        
        ASSERT_TRUE(appendResult);
        ASSERT_FALSE(segmentIndex.empty());
        if (segment_bytes <= segmentMaxSize)
            segmentIndexs.push_back(segmentIndex);
    }

    for (auto segmentIndex : segmentIndexs)
    {
        bool removeRes = segmentStore->RemoveAsync(segmentIndex);
        ASSERT_TRUE(removeRes);
    }

    i = 1;
    for (auto segmentIndex : segmentIndexs)
    {
        std::string key = SizeString::ToHexString(i++), readValue;
        bool readResult = segmentStore->Read(segmentIndex, key, readValue);
        ASSERT_TRUE(readResult);
    }

    sleep(SEGMENT_CLEAN_DELAY_IN_SECONDS + 1);
    
    // i = 1;
    // for (auto segmentIndex : segmentIndexs)
    // {
    //     std::string key = SizeString::ToHexString(i++), readValue;
    //     bool readResult = segmentStore->Read(segmentIndex, key, readValue);
    //     ASSERT_TRUE(readResult);
    // }

    // sleep(2);

    i = 1;
    for (auto segmentIndex : segmentIndexs)
    {
        std::string key = SizeString::ToHexString(i++), readValue;
        bool readResult = segmentStore->Read(segmentIndex, key, readValue);
        ASSERT_FALSE(readResult);
    }

    LOG(INFO) << "case over.";
    
}