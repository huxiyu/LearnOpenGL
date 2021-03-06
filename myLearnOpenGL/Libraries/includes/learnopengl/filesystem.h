#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include "root_directory.h" // This is a configuration file generated by CMake.

class FileSystem
{
public:
static std::string getPath(const std::string& path)
  {
    return std::string(logl_root) + std::string("/") + path;
  }

};

// FILESYSTEM_H
#endif
