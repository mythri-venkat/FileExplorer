#include "CommandMode.h"


mode_t defModeDir = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;

vector<string> getArgs(string command){
    //cout << "cm:"<<command;
    int idxs = command.find(" ");
    vector<string> args;
    
    if(idxs != string :: npos){
        string cmd = command.substr(0,idxs);
        args.push_back(cmd);
        int idx1 = command.find(" ",idxs+1);

        if(idx1 != string :: npos){
            args.push_back(command.substr(idxs+1,idx1-idxs-1));
            int idxend = command.find_first_of(" ",idx1+1);
            if(idxend == string :: npos){
                args.push_back(command.substr(idx1+1,command.length()-idx1-1));
            }
            else{
                args.push_back("invalid");
                args.push_back("invalid");
            }
        }
        else{
            args.push_back(command.substr(idxs+1,command.length()-idxs-1));
        }
        
    }
    else{
        args.push_back(command);
    }
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
        listdir(currentpath.c_str());
        printList(0,nFiles);
        return "";           
    }
    else{
        return "Cannot create dir";
    }
   
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
    else if(cmd == "cp"){
        cout << "1:"<<args[1]<<" 2:"<<args[2];
    }
    else if(cmd == "create_dir"){
        if(args.size() == 3)
        {
            strstat = createDir(args[1],args[2]);
            
        }
    }
    //printList(startidx,endidx);
    //writeStat(strstat);
    if(strstat == ""){
        moveCursor(rows+1,1);
    }
}

