#include "CommandMode.h"
#include <fcntl.h>

mode_t defModeDir = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;


string command="";
char homepath[PATH_MAX];
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
        parseCommand(command);
        command="";
        curPositionCmd=2;
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
        return "Cannot create dir";
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
        return "Cannot create file";
    }
    return "";
}

string copyFile(string src,string dest){
    int isrc = open(src.c_str(),O_RDONLY,0);
    if(isrc == -1){
        return "Cannot open source.";
    }
    mode_t fmode = GetFileAttributes(src.c_str(),true).mode;
    int idest = open(dest.c_str(),O_WRONLY|O_CREAT,fmode);
    if(idest == -1){
        return "Cannot open dest.";
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

string copyFiles(vector<string> args){
    string dest = args[args.size()-1];
    string str;
    if(dest.find("~") != string :: npos){
        str = getpwuid(getuid())->pw_dir;
        str+="/"+(dest!="~"?dest.substr(1,dest.length()-1):"")+"/";
    }
    else{
        str = dest+"/";
    }
    string err="";
    for(int i=1;i<args.size()-1;i++){
        err=copyFile(args[i],str+args[i]);
        if(err != ""){
            return err;
        }
    }
   
    
    return err;
}


string deletFile(string path){
    string str=homepath;
    str+="/"+path;
    int stat = remove(str.c_str()); 
    if(stat == -1){
        return "Cannot delete file:"+str;
    }       
    
    return "";
}

string rename(string oldfile,string newfile){
    if(rename(oldfile.c_str(),newfile.c_str()) == -1){
        return "Cannot rename.";
    }
    return "";
}

string moveFiles(vector<string> args){
    string dest = args[args.size()-1];
    string str;
    if(dest.find("~") != string :: npos){
        str = getpwuid(getuid())->pw_dir;
        str+="/"+(dest!="~"?dest.substr(1,dest.length()-1):"")+"/";
    }
    else{
        str = dest+"/";
    }
    string err="";
    for(int i=1;i<args.size()-1;i++){
        err=rename(args[i],str+args[i]);
        if(err != ""){
            return err;
        }
    }
   
    
    return err;

}

void parseCommand(string command){
    vector<string> args = getArgs(command);
    
    string strstat ="";
    if(args.size()==0)
        return;
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
            strstat = deletFile(args[1]);
            
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
    
    
}

