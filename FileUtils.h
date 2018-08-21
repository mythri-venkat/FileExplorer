#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;

#ifndef INC_FILEUTILS_H
#define INC_FILEUTILS_H

struct FileAttrib
{
    
    string ModifiedDate;
    string Permissions;
    string size;
    string owner;
    string group;
};

struct FileAttrib GetFileAttributes(const char * filename);

void listdir(const char *name);

#endif  