#include "log_storage.hpp"
#include "log_storage_test_helper.hpp"

TEST_F(LogStorageTests, TestLogStorageInit)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto log_storage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = log_storage->Init(logStorageOptions);
    ASSERT_TRUE(res);
}

TEST_F(LogStorageTests, TestLogStoragePut)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto log_storage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = log_storage->Init(logStorageOptions);
    
    std::string key;
    int i = 1;
    key.append((char *)&i, sizeof(size_t));

    std::string value = "value1";
    bool putRes = log_storage->Put(key, value);
    ASSERT_TRUE(putRes);
}

TEST_F(LogStorageTests, TestLogStorageGet)
{
    auto logIndexDb = ShortLife::CreateInstance<LogIndexDB>();
    auto logSegmentStore = ShortLife::CreateInstance<LogSegmentStore>();
    auto log_storage = ShortLife::CreateInstance<DataBaseLogStorage>(logIndexDb, logSegmentStore);
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {LogStoragePath, 1024, sizeof(instance_id_t)};
    bool res = log_storage->Init(logStorageOptions);
    
    std::string key;
    LOG(INFO)<<"key: "<<key;
    instance_id_t i = 1234;
    key.append((char *)&i, sizeof(instance_id_t));
    LOG(INFO)<<"key: "<<key;
    LOG(INFO)<<"key length: "<<key.size();

    std::string putValue = "value1";
    bool putRes = log_storage->Put(key, putValue);

    std::string getValue;
    bool getRes = log_storage->Get(key, getValue);
    ASSERT_TRUE(getRes) << "key: " << key;
    ASSERT_EQ(putValue, getValue);
}