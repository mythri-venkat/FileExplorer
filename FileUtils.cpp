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
#include <vector>
#include <algorithm>
#include <cstring>
#include "FileUtils.h"
using namespace std;

bool operator < (const FileAttrib& a,const FileAttrib& b){
    return a.Name < b.Name;
}

vector<FileAttrib> vFiles;


struct FileAttrib GetFileAttributes(const char * filename){

 struct stat sb;
 struct FileAttrib f;
 char ch[2000];
 strcat(ch,filename);
 strcat(ch,":stat");
 if (stat(filename, &sb) == -1) {
        perror(ch);
        //return f;
    }

    
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
    f.Name = filename;
    return f;

}



void listdir(const char *name)
{
    
    vFiles.clear();
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    
    while ((entry = readdir(dir)) != NULL) {      
	
	    struct FileAttrib fb=GetFileAttributes(entry->d_name);
        vFiles.push_back(fb);
        
    }
    closedir(dir);
    sort(vFiles.begin(),vFiles.end());

    printList(0,vFiles.size());
}

void printList(int start,int end){
    if(end > vFiles.size())
        end=vFiles.size();
    cout << "\033[2J\033[1;1H";
    for(unsigned int i=start;i<end;i++){
        FileAttrib fb = vFiles[i];
        cout << " "<<fb.Permissions<<"  "<<fb.owner<<"  "<<fb.group<<"  "<<fb.size<<"  "<<fb.ModifiedDate<<"  " <<fb.Name<<"\n";
    }
    cout << "\033[1;1H";
}

void enterDir(int n){
    if(n<vFiles.size()){
        char * name = new char[vFiles[n].Name.length()+1];
        strcpy(name,vFiles[n].Name.c_str());
        listdir(name);
    }
}