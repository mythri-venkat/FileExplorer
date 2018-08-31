#include "CommandMode.h"
#include <fcntl.h>
#include <ftw.h>

mode_t defModeDir = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;
string destCopy="";
string srcCopy="";
string command="";
string strsnapshot="";
stack<string> stSnapshot;
vector<FileAttrib> searchResults;

bool makeCommand(char charr){
    char ch=charr;
    //char ch2=charr[2];
    if(ch == ((char)27)){
        //if(charr[1] == '\0')
            return true;
        /*else if(charr[1] == '['){
            if(ch2=='C'){
                cout << "\033[1C";
                curPositionCmd++;
            }
            else if(ch2=='D'){
                cout << "\033[1D";
                curPositionCmd--;
            }

        }*/
    }
    else if( ch == ((char)127)){
        if(command!=""){
        cout << ((char)127);
        command.erase(curPositionCmd-3,1);
        curPositionCmd--;
        writeStatCmd(command);
        }
        else{
            writeStatCmd("");
            curPositionCmd = 2;
        }
    }
    else if(ch == '\b' ){
        cout << '\b';
        command.erase(curPositionCmd,1);
        writeStatCmd(command);
    }
    else if(ch == '\n'){
        bool switchtonormal= parseCommand(command);
        command="";
        curPositionCmd=2;
        return switchtonormal;
    }
    else{
        curPositionCmd++;
        command+=ch;
        writeStatCmd(command);

    }
    return false;
}

vector<string> getArgs(string command){
    //cout << "cm:"<<command;
    int idxfirst =0;
    int idxlast = command.find(" ");
    
    vector<string> args;
    
    while(idxlast != string :: npos){
        
        string cmd = command.substr(idxfirst,idxlast-idxfirst);
        args.push_back(cmd);
        idxfirst=idxlast+1;
        idxlast = command.find(" ",idxfirst);
        while(idxlast != string::npos && command[idxlast-1]=='\\'){
            command.erase(idxlast-1,1);
            idxlast = command.find(" ",idxlast);
        }        
    }
    idxlast = idxlast== string::npos ? command.length()-1:idxlast;
    args.push_back(command.substr(idxfirst,idxlast-idxfirst+1));
        
    
    return args;
}
string createDir(string dirname,string path){
    string str;
    if(path.find("~") != string :: npos){
        str = getpwuid(getuid())->pw_dir;
        str+="/"+(path!="~"?path.substr(1,path.length()-1):"")+"/"+dirname;
    }
    else{
        str = path+"/"+dirname;
    }
    
    const int err = mkdir(str.c_str(),defModeDir);
    if(err!= -1){
        /*listdir(currentpath.c_str());
        printList(0,nFiles);
        return ""; */        
        return "";  
    }
    else{
        return strerror(errno);
    }
   
}

string createFile(string filename,string path){

    string str;
    if(path.find("~") != string :: npos){
        str = getpwuid(getuid())->pw_dir;
        str+="/"+(path!="~"?path.substr(1,path.length()-1):"")+"/"+filename;
    }
    else{
        str = path+"/"+filename;
    }
    if(open(str.c_str(), O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1){
        return strerror(errno);
    }
    return "";
}

string copyFile(string src,string dest,mode_t fmode){
    
    int isrc = open(src.c_str(),O_RDONLY,0);
    if(isrc == -1){
        return strerror(errno);
    }
    
    int idest = open(dest.c_str(),O_WRONLY|O_CREAT,fmode);
    if(idest == -1){
        return strerror(errno);
    }
    char buffer[BUFSIZ];
    size_t size;
    while((size=read(isrc,buffer,BUFSIZ))>0 ){
        write(idest,buffer,size);
    }
    close(isrc);
    close(idest);
    return "";
}

int copyCallback(const char *path, const struct stat *sb, int typeflag, struct FTW *ptftw){
    string dest =destCopy+ path;
    //cout << "path:"<<path<<"    dest:"<<destCopy<<"   ";
    if(typeflag == FTW_D){
        return mkdir(dest.c_str(),sb->st_mode);
    }
    else{
        if(copyFile(path,dest.c_str(),sb->st_mode)==""){
            return 0;
        }        
    }
    return -1;
}



string copyFiles(vector<string> args){
    string dest = args[args.size()-1];
    string str;
    if(dest.find("~") != string :: npos){
        str = homepath;
        str+="/"+(dest!="~"?dest.substr(1,dest.length()-1):"")+"/";
    }
    else{
        str = dest+"/";
    }
    string err="";
    for(int i=1;i<args.size()-1;i++){
        FileAttrib fb = GetFileAttributes(args[i].c_str(),true);
        if(fb.type == Dir){
            //chdir(str.c_str());
            destCopy = str;
            int eno = nftw((args[i]).c_str(),copyCallback,128,FTW_PHYS);
            if(eno == -1){
                return strerror(errno);
            }
        }
        else{
            err=copyFile(args[i],str+args[i],fb.mode);
            if(err != ""){
                return err;
            }
        }
    }
   
    
    return err;
}


int delete_callback(const char *path, const struct stat *sb, int typeflag, struct FTW *ptftw){
    if(typeflag == FTW_DP){
        return rmdir(path);
    }
    else if(typeflag == FTW_F || typeflag == FTW_SL || typeflag == FTW_SLN){
        return unlink(path);
    }
    return -1;
}

string deleteDir(string path){
    string str=homepath;
    str+="/"+path;
    int stat=0;
    
    stat = nftw(str.c_str(), delete_callback, 128, FTW_DEPTH | FTW_PHYS);
    if(stat == -1){
        return strerror(errno);
    }   
    
    return "";
}

string deleteFile(string path){
    string str=homepath;
    str+="/"+path;
    int stat = remove(str.c_str());
    if(stat == -1){
        return strerror(errno);
    }   
    
    return "";
}

string rename(string oldfile,string newfile){
    if(oldfile[0] == '/'){
        oldfile=homepath+oldfile;
    }
    if(rename(oldfile.c_str(),newfile.c_str()) == -1){
        return strerror(errno);
    }
    return "";
}


string moveFiles(vector<string> args){
    string dest = args[args.size()-1];
    string str;
    if(dest.find("~") != string :: npos){
        str = homepath;
        str+="/"+(dest!="~"?dest.substr(1,dest.length()-1):"")+"/";
    }
    else{
        str = dest+"/";
    }
    string err="";
    for(int i=1;i<args.size()-1;i++){
        FileAttrib fb = GetFileAttributes(args[i].c_str(),true);
        if(fb.type == Dir){
            err=rename("./"+args[i],str+args[i]);
            if(err != ""){
                return err;
            }
        }
        else{
            err=rename(args[i],str+args[i]);
            if(err != ""){
                return err;
            }
        }
    }
   
    
    return err;

}

void gotoCommandMode(string path){
    string str="";
    if(path[0] == '/'){
        str = homepath+path;
    }
    else{
        str=currentpath+"/"+path;
    }
    gotoDir(str,true);
}

string listSnapshot(string name)
{
    DIR *dir;
    struct dirent *entry;
    string strfilename;
    
    if (!(dir = opendir(name.c_str())))
        return strerror(errno);

    strsnapshot+="\n\n"+name+":\n";

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            string str = name +"/"+entry->d_name;
            strfilename = entry->d_name;
            strsnapshot+=strfilename+"\t";
            stSnapshot.push(str);
            
        } 
        else {
            strfilename = entry->d_name;
            strsnapshot+=strfilename+"\t";
        }
    }
    closedir(dir);
    string err="";
    while(!stSnapshot.empty()){
        string nxtpath = stSnapshot.top();
        stSnapshot.pop();
        err= listSnapshot(nxtpath);
    }
    return err;
}

string snapshot(string folder,string dumpfile){
    int fd;
    string err;
    strsnapshot="";
    if((err=listSnapshot(folder)) != ""){
        return err;
    }
    if(strsnapshot.size()==0){
        return "Could not create snapshot";
    }
    if((fd=open(dumpfile.c_str(),O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
        return strerror(errno);
    }
    const char * buf = strsnapshot.c_str();
    if(write(fd,buf,strlen(buf)) == -1){
        close(fd);
        return strerror(errno);
    }
    close(fd);
    
    return "";
}

string search(string path,string value)
{
    DIR *dir;
    struct dirent *entry;
    string strfilename;
    string err="";
    if (!(dir = opendir(path.c_str())))
        return strerror(errno);


    while ((entry = readdir(dir)) != NULL) {
        //cout << "entry:"<<entry->d_name<<" ";
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            string str = path +(path[path.length()-1]=='/'?"":"/")+entry->d_name;
            //strfilename = entry->d_name;
            if(strcmp(entry->d_name,value.c_str())==0){
                FileAttrib fa=GetFileAttributes(str.c_str(),true);
                fa.path = str;
                fa.fullpath = true;
                fa.Name = entry->d_name;
                searchResults.push_back(fa);
            }
            err= search(str,value);
        } 
        else {
            strfilename = entry->d_name;
            string str = path +(path[path.length()-1]=='/'?"":"/")+entry->d_name;
            if(strcmp(entry->d_name,value.c_str())==0){
                FileAttrib fa=GetFileAttributes(str.c_str(),true);
                fa.Name = entry->d_name;
                fa.path = str;
                fa.fullpath=true;
                searchResults.push_back(fa);
            }
        }
    }
    closedir(dir);
    
    return err;
}

string searchName(string name){
    searchResults.clear();
    string err;
    if((err=search(currentpath,name)) == ""){
        if(searchResults.size()==0){
            err="No results found.";
        }
        else{
            vFiles.clear();
            vFiles = searchResults;
        }
    }
    searchResults.clear();
    return err;
}

bool parseCommand(string command){
    vector<string> args = getArgs(command);
    
    string strstat ="";
    if(args.size()==0)
        return false;
    string cmd = args[0];
    if(cmd == "q"){
        exit(0);
    }
    else if(cmd == "copy"){
        if(args.size() >=3){
            strstat = copyFiles(args);
        }        
    }
    else if(cmd == "create_dir"){
        if(args.size() == 3)
        {
            strstat = createDir(args[1],args[2]);
            
        }

    }
    else if(cmd == "create_file"){
        if(args.size() == 3)
        {
            strstat = createFile(args[1],args[2]);
            
        }

    }
    else if(cmd == "rename"){
        if(args.size() == 3)
        {
            strstat = rename(args[1],args[2]);
            
        }

    }
    else if(cmd == "move"){
        if(args.size() >=3){
            strstat = moveFiles(args);
        }        
    }
    else if(cmd == "delete_file"){
        if(args.size() == 2)
        {
            strstat = deleteFile(args[1]);
            
        }

    }
    else if(cmd == "delete_dir"){
        if(args.size() == 2)
        {
            strstat = deleteDir(args[1]);
            
        }

    }
    else if(cmd == "goto"){
        if(args.size()==2){
            gotoCommandMode(args[1]);
        }
    }
    else if(cmd == "snapshot"){
        if(args.size()==3){
            strstat=snapshot(args[1],args[2]);
        }
    }
    else if(cmd == "search"){
        if(args.size()==2){
            if((strstat=searchName(args[1]))==""){
                startidx=0;
                endidx = nFiles;
                if(endidx > rows)
                    endidx = rows;
                printList(0,endidx);
                return true;
            }
        }
    }
    //printList(startidx,endidx);
    listdir(currentpath.c_str());
    startidx=0;
    endidx = nFiles;
    if(endidx > rows)
        endidx = rows;
    printList(0,endidx);
    writeStatCmd(strstat);
    
    return false;
    
}

