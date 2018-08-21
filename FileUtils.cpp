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
#include "FileUtils.h"
using namespace std;


struct FileAttrib GetFileAttributes(const char * filename){
 struct stat sb;
 if (stat(filename, &sb) == -1) {
        perror("stat");
        //return sb;
    }

    struct FileAttrib f;
    f.ModifiedDate = ctime(&sb.st_mtime);
    
    f.ModifiedDate.erase(f.ModifiedDate.find_last_not_of(" \n\r\t")+1);
    long long size=(long long)sb.st_size;
    if(size < 1024){
        f.size = to_string(size)+"B";
    }
    else if(size < 1024*1024){
        f.size = to_string(size/1024)+"KB";
    }
    else if(size < 1024 * 1024*1024){
        f.size = to_string(size/(1024*1024))+"MB";
    }
    else{
        f.size = to_string(size/(1024*1024*1024))+"GB";
    } 
    struct passwd * pass = getpwuid(sb.st_uid);
    f.owner = pass->pw_name;
    struct group * grp = getgrgid(sb.st_gid);
    f.group = grp->gr_name;
    f.Permissions = sb.st_mode & S_IRUSR ? "r" : "-";
    f.Permissions+=sb.st_mode & S_IWUSR ? "w" : "-";
    f.Permissions+=sb.st_mode & S_IXUSR ? "x" : "-";
    f.Permissions+=sb.st_mode & S_IRGRP ? "r" : "-";
    f.Permissions+=sb.st_mode & S_IWGRP ? "w" : "-";
    f.Permissions+=sb.st_mode & S_IXGRP ? "x" : "-";
    f.Permissions+= sb.st_mode & S_IROTH ? "r" : "-";
    f.Permissions+=sb.st_mode & S_IWOTH ? "w" : "-";
    f.Permissions+=sb.st_mode & S_IXOTH ? "x" : "-";

    return f;

}

void listdir(const char *name)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    /*if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
        } else */
    while ((entry = readdir(dir)) != NULL) {      
	
	    struct FileAttrib fb=GetFileAttributes(entry->d_name);
        cout << fb.Permissions<<"  "<<fb.owner<<"  "<<fb.group<<"  "<<fb.size<<"  "<<fb.ModifiedDate<<"  " <<entry->d_name<<"\n";
        
    }
    closedir(dir);
}