#ifndef LOG_STORAGE_TEST_HELPER_HPP
#define LOG_STORAGE_TEST_HELPER_HPP

#include <string>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "fs_helper.hpp"

static const std::string &LogStoragePath = "/tmp/logstorage";


class LogStorageTests : public ::testing::Test
{
protected:
    virtual void TearDown() override;
};

void LogStorageTests::TearDown()
{
    LOG(INFO) << "rm -rf " << LogStoragePath;
    FSHelper::rmrf(LogStoragePath.c_str());
}


#endif