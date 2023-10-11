#ifndef FS_HELPER_HPP
#define FS_HELPER_HPP

#include <string>
#include <ftw.h>
#include <fstream>
#include <unistd.h>

class FSHelper
{
public:
    static size_t FileSize(const std::string &path);
    static bool DirExists(const std::string &path);
    static bool DirExistsOrCreate(const std::string &path);
    static bool DirClean(const std::string &path);
    static int rmrf(const char *path);
};

#endif
