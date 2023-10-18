#include "log_storage.hpp"
#include "log_storage_test_helper.hpp"

TEST_F(LogStorageTests, TestLogStorageInit)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto logStorage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = logStorage->Init(logStorageOptions);
    ASSERT_TRUE(res);
}

TEST_F(LogStorageTests, TestLogStoragePut)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto logStorage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = logStorage->Init(logStorageOptions);
    
    std::string key;
    int i = 1;
    key.append((char *)&i, sizeof(size_t));

    std::string value = "value1";
    bool putRes = logStorage->Put(key, value);
    ASSERT_TRUE(putRes);
}

TEST_F(LogStorageTests, TestLogStorageGet)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto logStorage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = logStorage->Init(logStorageOptions);
    
    std::string key;
    instance_id_t i = 1234;
    key.append((char *)&i, sizeof(instance_id_t));

    std::string putValue = "value1";
    bool putRes = logStorage->Put(key, putValue);

    std::string getValue;
    bool getRes = logStorage->Get(key, getValue);
    ASSERT_TRUE(getRes) << "key: " << key;
    ASSERT_EQ(putValue, getValue);
}

TEST_F(LogStorageTests, TestLogStorageDelete)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto logStorage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = logStorage->Init(logStorageOptions);
    
    std::string key;
    instance_id_t i = 1234;
    key.append((char *)&i, sizeof(instance_id_t));

    std::string putValue = "value1";
    bool putRes = logStorage->Put(key, putValue);

    std::string getValue;
    bool getRes = logStorage->Get(key, getValue);
    ASSERT_TRUE(getRes) << "key: " << key;
    ASSERT_EQ(putValue, getValue);

    bool delRes = logStorage->Delete(key);  
    ASSERT_TRUE(delRes);

    getRes = logStorage->Get(key, getValue);
    ASSERT_FALSE(getRes) << "key: " << key;

    delRes = logStorage->Delete(key);
    ASSERT_FALSE(delRes);
}

TEST_F(LogStorageTests, InitWithExistStorage)
{
    
}
