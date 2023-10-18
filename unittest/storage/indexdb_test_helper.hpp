#ifndef INDEXDB_TEST_HELPER_HPP
#define INDEXDB_TEST_HELPER_HPP

#include <gtest/gtest.h>
#include <glog/logging.h>
#include "fs_helper.hpp"

static const std::string &DBPath = "/tmp/indexdb_test/test";
static const std::string &SegmentPath = "/tmp/segment_test";


class IndexDbTests : public ::testing::Test
{
private:
    /* data */
public:
    IndexDbTests(/* args */);
    ~IndexDbTests();

protected:
    virtual void TearDown() override;
};

IndexDbTests::IndexDbTests(/* args */)
{
}

IndexDbTests::~IndexDbTests()
{
}

void IndexDbTests::TearDown()
{
    LOG(INFO) << "rm -rf " << DBPath;
    FSHelper::rmrf(DBPath.c_str());
}

#endif