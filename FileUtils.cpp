
#include "FileUtils.h"
using namespace std;

//operator to compare name of iles in fileattrib struct.
bool operator<(const FileAttrib &a, const FileAttrib &b)
{
    return a.Name < b.Name;
}

vector<FileAttrib> vFiles;
string currentpath;
int nFiles;
stack<string> backStack;
stack<string> fwdStack;

char homepath[PATH_MAX];

//get attributs of file required for listing.fullpath indicates filename passed is absolute otherwise relative.
FileAttrib GetFileAttributes(const char *filename, bool fullpath)
{

    struct stat sb;
    struct FileAttrib f;
    string str = "";
    int temp1 = strcmp(filename, ".");
    int temp2 = strcmp(filename, "..");
    str = filename;
    if (temp1 != 0 && temp2 != 0 && !fullpath)
    {
        str = currentpath + "/" + filename;
    }
    if (stat(str.c_str(), &sb) == -1)
    {
        cout << endl
             << (filename) << endl;
        return f;
    }

    f.ModifiedDate = ctime(&sb.st_mtime);

    f.ModifiedDate.erase(f.ModifiedDate.find_last_not_of(" \n\r\t") + 1);
    long long size = (long long)sb.st_size;
    if (size < 1024)
    {
        f.size = to_string(size) + "B";
    }
    else if (size < 1024 * 1024)
    {
        f.size = to_string(size / 1024) + "KB";
    }
    else if (size < 1024 * 1024 * 1024)
    {
        f.size = to_string(size / (1024 * 1024)) + "MB";
    }
    else
    {
        f.size = to_string(size / (1024 * 1024 * 1024)) + "GB";
    }
    struct passwd *pass = getpwuid(sb.st_uid);
    f.owner = pass->pw_name;
    struct group *grp = getgrgid(sb.st_gid);
    f.group = grp->gr_name;
    f.type = None;
    if (S_ISDIR(sb.st_mode))
    {
        f.type = Dir;
        f.Permissions = "d";
    }
    else if (S_ISREG(sb.st_mode))
    {
        f.type = RegularFile;
        f.Permissions = "-";
    }
    else if (S_ISBLK(sb.st_mode))
    {
        f.type = Block;
        f.Permissions = "b";
    }
    else if (S_ISCHR(sb.st_mode))
    {
        f.type = CharacterDev;
        f.Permissions = "c";
    }
    else if (S_ISLNK(sb.st_mode))
    {
        f.type = Link;
        f.Permissions = "l";
    }
    else if (S_ISSOCK(sb.st_mode))
    {
        f.type = SocketFile;
        f.Permissions = "s";
    }
    else
    {
        f.type = Pipe;
        f.Permissions = "p";
    }

    f.Permissions += sb.st_mode & S_IRUSR ? "r" : "-";
    f.Permissions += sb.st_mode & S_IWUSR ? "w" : "-";
    f.Permissions += sb.st_mode & S_IXUSR ? "x" : "-";
    f.Permissions += sb.st_mode & S_IRGRP ? "r" : "-";
    f.Permissions += sb.st_mode & S_IWGRP ? "w" : "-";
    f.Permissions += sb.st_mode & S_IXGRP ? "x" : "-";
    f.Permissions += sb.st_mode & S_IROTH ? "r" : "-";
    f.Permissions += sb.st_mode & S_IWOTH ? "w" : "-";
    f.Permissions += sb.st_mode & S_IXOTH ? "x" : "-";
    f.Name = filename;
    f.path = str;
    f.mode = sb.st_mode;
    f.fullpath = false;
    return f;
}

//lists all the files in the directory and saves them in global variable vFiles.
void listdir(const char *name)
{

    vFiles.clear();
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
    {
        if (!(dir = opendir(homepath)))
        {
            return;
        }
    }

    while ((entry = readdir(dir)) != NULL)
    {

        struct FileAttrib fb = GetFileAttributes(entry->d_name, false);
        vFiles.push_back(fb);
    }
    closedir(dir);
    chdir(name);
    sort(vFiles.begin(), vFiles.end());
    nFiles = vFiles.size();
}

//refresh/prints the list of contents of dir from given start to end index.
void printList(int start, int end)
{
    if (end > vFiles.size())
        end = vFiles.size();
    cout << "\033[2J\033[1;1H";
    for (unsigned int i = start; i < end; i++)
    {
        FileAttrib fb = vFiles[i];
        //cout << " "<<fb.Permissions<<"  "<<fb.owner<<"  "<<fb.group<<"  "<<fb.size<<"  "<<fb.ModifiedDate<<"  " << (fb.Name)<<"\n";
        cout << " " << fb.Permissions << "  " << fb.owner << "  " << fb.group << "  " << fb.size << "  " << fb.ModifiedDate << "  " << (fb.fullpath ? fb.path : fb.Name) << "\n";
    }
    cout << "\033[1;1H";
}

//open file using xdg-open
string openFile(string filename, bool fullpath)
{
    string strfile = filename;
    if (!fullpath)
        strfile = currentpath + "/" + filename;
    pid_t pid = fork();
    int status;
    if (pid == 0)
    {
        //cout << "strfile:"<<strfile;
        execl("/usr/bin/xdg-open", "xdg-open", strfile.c_str(), (char *)0);
        exit(0);
    }
    else
    {
        if (waitpid(pid, &status, 0) > 0)
        {

            int res = WIFEXITED(status);
            if (res && !WEXITSTATUS(status))
            {
                return "";
            }
            else if (WIFEXITED(status) && WEXITSTATUS(status))
            {
                return "Could not open file.";
            }
            else
                return ("program didn't terminate normally.");
        }
        else
        {
            return "Could not open file";
        }
    }
    return "";
}

//called when enterkey is pressed.list files if the selected line corresponds to dir else open file.return type of file and status msg.
pair<FileType, string> enterDir(int n)
{
    pair<FileType, string> pout;
    FileType t = None;
    string msg = "";
    if (n < vFiles.size())
    {
        t = vFiles[n].type;
        if (vFiles[n].Name != ".")
        {

            if (t == Dir)
            {
                if (vFiles[n].Name == "..")
                {
                    currentpath.erase(currentpath.find_last_of('/'), string::npos);
                }
                else
                {
                    if (vFiles[n].fullpath)
                    {
                        currentpath = homepath+vFiles[n].path;
                    }
                    else
                    {
                        currentpath = currentpath + "/" + vFiles[n].Name;
                    }
                }
                //currentpath=currentpath+"/"+ vFiles[n].Name;
                //msg ="path:"+ currentpath;
                backStack.push(currentpath);
                listdir(currentpath.c_str());
                //msg = currentpath;
            }
            else if (t == RegularFile)
            {
                msg = openFile(vFiles[n].fullpath ? vFiles[n].path : vFiles[n].Name, vFiles[n].fullpath);
            }
        }
    }
    return make_pair(t, msg);
}

/*void addToList(string str){
    if(str.substr(0,1)=="/"){
        int idx = str.find(currentpath);
        if(idx != string::npos && str.find("/",currentpath.length()+1)==string::npos){
            
        }
    }
    else if(str.find("/")==string :: npos || str.find("/")==str.length()-1){
        listdir(currentpath.c_str());
        printList(0,vFiles.size());    
    }
}*/
