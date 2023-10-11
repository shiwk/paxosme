#include "fs_helper.hpp"

 size_t FSHelper::FileSize(const std::string &path)
{
    std::ifstream testFile(path, std::ios::binary);
    const auto begin = testFile.tellg();
    testFile.seekg(0, std::ios::end);
    const auto end = testFile.tellg();
    const auto fsize = (end - begin);
    return fsize;
}

 bool FSHelper::DirExists(const std::string &path)
{
    struct stat path_info;
    const char *cpath = path.c_str();
    return stat(cpath, &path_info) == 0;
}

 bool FSHelper::DirExistsOrCreate(const std::string &path)
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

 bool FSHelper::DirClean(const std::string &path)
{
    struct stat path_info;
    const char *cpath = path.c_str();
    if (stat(cpath, &path_info) != 0)
    {
        return true;
    }
    if (rmdir(path.c_str()) == -1)
    { // Remove the directory
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

int FSHelper::rmrf(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}
