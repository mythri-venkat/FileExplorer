
#include "FileUtils.h"
using namespace std;

bool operator < (const FileAttrib& a,const FileAttrib& b){
    return a.Name < b.Name;
}

vector<FileAttrib> vFiles;
string currentpath;
int nFiles;
stack<string> backStack;
stack<string> fwdStack;

FileAttrib GetFileAttributes(const char * filename){

 struct stat sb;
 struct FileAttrib f;
 string str="";
 int temp1 =  strcmp(filename,".") ;
 int temp2 = strcmp(filename,"..");
 str=filename;
 if(temp1 != 0 && temp2 != 0 ){
     str = currentpath + "/"+filename;
 }
 if (stat(str.c_str(), &sb) == -1) {
        cout <<endl<<(filename)<<endl;
        return f;
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
    f.path = str;
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
    nFiles = vFiles.size();
    
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
       
        if(vFiles[n].Name != "."){
        currentpath=currentpath+"/"+ vFiles[n].Name;
        backStack.push(currentpath);
        listdir(currentpath.c_str());
        
        }
    }
}