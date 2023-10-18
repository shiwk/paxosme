#include "indexdb_test_helper.hpp"
#include "index_db.hpp"
#include "common.hpp"

TEST_F(IndexDbTests, TestIndexDbInit)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = DBPath;
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    ASSERT_TRUE(log_index_db->Init(log_storage_options));
}

TEST_F(IndexDbTests, TestPutIndex)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = DBPath;
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    log_index_db->Init(log_storage_options);

    bool putRes = log_index_db->PutIndex("key1", "value1");
    ASSERT_TRUE(putRes);
}

TEST_F(IndexDbTests, TestGetIndex)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = DBPath;
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    log_index_db->Init(log_storage_options);
    std::string getValue;

    bool getRes = log_index_db->GetIndex("key1", getValue);
    ASSERT_FALSE(getRes);
    ASSERT_EQ("", getValue);

    bool putRes = log_index_db->PutIndex("key1", "value1");
    ASSERT_TRUE(putRes);

    getRes = log_index_db->GetIndex("key1", getValue);
    ASSERT_TRUE(getRes);
    ASSERT_EQ("value1", getValue);
}

TEST_F(IndexDbTests, TestDelIndex)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = DBPath;
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    log_index_db->Init(log_storage_options);
    std::string getValue;

    bool delRes = log_index_db->DelIndex("key2");
    ASSERT_TRUE(delRes);

    bool putRes = log_index_db->PutIndex("key2", "value2");
    ASSERT_TRUE(putRes);

    bool getRes = log_index_db->GetIndex("key2", getValue);
    ASSERT_TRUE(getRes);
    ASSERT_EQ("value2", getValue);

    delRes = log_index_db->DelIndex("key2");
    ASSERT_TRUE(delRes);

    getRes = log_index_db->GetIndex("key2", getValue);
    ASSERT_FALSE(getRes);
}


TEST_F(IndexDbTests, TestGetLastIndex)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = DBPath;
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    log_index_db->Init(log_storage_options);
    size_t start = 0;
    size_t end = 200;

    for (size_t i = 0; i <= end; i++)
    {
        std::string key;
        key.append((char *)&i, sizeof(size_t));
        std::string value = "value" + std::to_string(i);
        bool putRes = log_index_db->PutIndex(key, value);
        ASSERT_TRUE(putRes);

        std::string getValue;
        bool getRes = log_index_db->GetIndex(key, getValue);
        ASSERT_TRUE(getRes);
        ASSERT_EQ(value, getValue);

        std::string getLastKey;
        bool getLastRes = log_index_db->GetLastIndex(getLastKey, getValue);
        ASSERT_TRUE(getLastRes);
        ASSERT_EQ(key, getLastKey) << "i: " << i;
        ASSERT_EQ(value, getValue) << "i: " << i;
    }
}