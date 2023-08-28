#include <gtest/gtest.h>
#include <segment_store.hpp>

TEST(TestAddIntExample, test_add_int) {
  int res = 10 + 24;
  EXPECT_EQ(res, 34);
}

TEST(TestSegmentStoreTests, SegmentStoreInit)
{
    // std::shared_ptr<LogSegmentStore> semgmentStore(LogSegmentStore::New());
    auto semgmentStore = LongLife::CreateInstance<LogSegmentStore>();
    paxosme::LogStorage::LogStorageOptions logStorageOptions = {"/tmp/segment_dir", 1024};
    bool initResult = semgmentStore->Init(logStorageOptions);
    EXPECT_TRUE(initResult);
}