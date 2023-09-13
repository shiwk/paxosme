#ifndef TESTCASE_ENV_HPP
#define TESTCASE_ENV_HPP

#include <gtest/gtest.h>
#include <unistd.h>
#include <ftw.h>
#include <fstream>
#include <glog/logging.h>

const std::string &DirPath = "/tmp/segment_test";

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
        LOG(INFO) << "mkdir "<< DirPath;
        DirExistsOrCreate(DirPath);
    }

    virtual void TearDown() override
    {
        LOG(INFO) << "rm -rf "<< DirPath;
        rmrf(DirPath.c_str());
    }

public:
    static size_t FileSize(const std::string &path)
    {
        std::ifstream testFile(path, std::ios::binary);
        const auto begin = testFile.tellg();
        testFile.seekg (0, std::ios::end);
        const auto end = testFile.tellg();
        const auto fsize = (end-begin);
        return fsize;
    }

    static bool DirExists(const std::string &path)
    {
        struct stat path_info;
        const char *cpath = path.c_str();
        return stat(cpath, &path_info) == 0;
    }
    
    static bool DirExistsOrCreate(const std::string &path)
    {
        struct stat path_info;
        const char *cpath = path.c_str();
        if (stat(cpath, &path_info) != 0)
        {
            // not exixts and create
            if (mkdir(cpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH /*Read, Write, Execute by owner and others*/) == -1)
            {
                // on err, create dir failed
                return false;
            }

            // printf("Create directory: %s\n", path.c_str());
        }
        else if (!(path_info.st_mode & S_IFDIR))
        {
            // on err, exists but not directory
            return false;
        }
        return true;
    }

    static bool DirClean(const std::string &path)
    {
        struct stat path_info;
        const char *cpath = path.c_str();
        if (stat(cpath, &path_info) != 0)
        {
            return true;
        }
        if (rmdir(path.c_str()) == -1) {  // Remove the directory
            // printf("Clean directory %s falied: %s", path.c_str(), strerror(errno));
            return false;
        }
        // printf("Clean directory success: %s\n", path.c_str());
        return true;
    }

    static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
    {
        int rv = remove(fpath);

        if (rv)
            perror(fpath);

        // printf("Clean directory success: %s\n", fpath);
        return rv;
    }

    static int rmrf(const char *path)
    {
        return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
    }

};

#endif