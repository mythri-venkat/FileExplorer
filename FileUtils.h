

#ifndef INC_FILEUTILS_H
#define INC_FILEUTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <sys/wait.h>
using namespace std;

enum FileType{
    Dir,
    RegularFile,
    Link,
    Block,
    CharacterDev,
    Pipe,
    SocketFile,
    None
};

struct FileAttrib
{
    string Name;
    string ModifiedDate;
    string Permissions;
    string size;
    string owner;
    string group;
    string path;
    FileType type;
};

extern stack<string> backStack;
extern stack<string> fwdStack;

extern string currentpath;

extern int nFiles;

pair<FileType,string> enterDir(int n);

void printList(int start,int end);

struct FileAttrib GetFileAttributes(const char * filename);

void listdir(const char *name);

#endif  