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