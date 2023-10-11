#include "indexdb_test_helper.hpp"
#include "index_db.hpp"
#include "common.hpp"

TEST_F(IndexDbTests, TestIndexDbInit)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = "/tmp/indexdb_test";
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    ASSERT_TRUE(log_index_db->Init(log_storage_options));
}

TEST_F(IndexDbTests, TestPutIndex)
{
    paxosme::LogStorage::LogStorageOptions log_storage_options;
    log_storage_options.dbPath = "/tmp/indexdb_test";
    auto log_index_db = ShortLife::CreateInstance<LogIndexDB>();
    log_index_db->Init(log_storage_options);

    bool putRes = log_index_db->PutIndex("key1", "value1");
    ASSERT_TRUE(putRes);
}