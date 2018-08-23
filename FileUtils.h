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
    string Name;
    string ModifiedDate;
    string Permissions;
    string size;
    string owner;
    string group;


};



void enterDir(int n);

void printList(int start,int end);

struct FileAttrib GetFileAttributes(const char * filename);

void listdir(const char *name);

#endif  