#ifndef TESTCASE_ENV_HPP
#define TESTCASE_ENV_HPP

#include <gtest/gtest.h>
#include <glog/logging.h>
#include "fs_helper.hpp"

const std::string &SegmentPath = "/tmp/segment_test";

class TestSegmentStoreTests : public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
    }

    static void TearDownTestCase()
    {
    }

protected:
    virtual void SetUp() override
    {
        LOG(INFO) << "mkdir " << SegmentPath;
        FSHelper::DirExistsOrCreate(SegmentPath);
    }

    virtual void TearDown() override
    {
        LOG(INFO) << "rm -rf " << SegmentPath;
        FSHelper::rmrf(SegmentPath.c_str());
    }
};

#endif