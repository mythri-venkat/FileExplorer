
#include"Events.h"
using namespace std;

int startidx=0,endidx=0;
int linenum=0;
int rows;
string currentStat;
const string strNormal = "-- NORMAL MODE --";
const string strCommand = "-- COMMAND MODE --";

bool cmdMode;
int curPositionCmd;

void moveCursor(int x,int y){
  cout << "\033["<<x<<";"<<y<<"H";
}

void writeStat(string str){
  int cursorpos = linenum - startidx +1;
  moveCursor(rows+1,1);
  cout << "\033[2K";
  cout << str;
  moveCursor(cursorpos,1);
  currentStat=str;
}

void writeStatCmd(string str){
  
  moveCursor(rows+1,1);
  cout << "\033[2K";
  cout << ":"+str;
  
  currentStat=str;
}

bool getWinSize(){
  struct winsize s;
  ioctl(STDOUT_FILENO,TIOCGWINSZ,&s);
  if(s.ws_row-1 != rows){
    rows=--s.ws_row;
    endidx = nFiles;
    if(endidx > rows){
      endidx = rows;
    }
    printList(0,endidx);
    moveCursor(1,1);
    writeStat(strNormal);
    linenum=0;
    return true;
  }
  return false;
}


void enterKey(){
  pair<FileType,string> pout = enterDir(linenum);
  if(pout.first == Dir){
    startidx =0;
    endidx = nFiles;
    if(endidx > rows){
      endidx=rows;
    }
    printList(startidx,endidx);
    linenum=0;
    writeStat(strNormal);
	  
  }
  else if(pout.first == RegularFile && pout.second != ""){
    printList(startidx,endidx);
    writeStat(pout.second);
  }
}

void upArrowKey(){
  /*if(getWinSize()){
    return;
  }*/
  if(linenum>startidx){
   	cout << "\033[1A";
   	linenum--;
  }
  else{
   if(linenum > 0){
      linenum --;
      startidx = linenum;
      endidx--;
      printList(startidx,endidx);
      writeStat(strNormal);
    }
  }
}

void downArrowKey(){
  /*if(getWinSize()){
    return;
  }*/
  
  if(linenum < endidx-1){
    cout << "\033[1B";
    linenum++;
  }
  else{
    if(linenum < nFiles-1){
       startidx++;
       endidx++;
       linenum=endidx-1;
       printList(startidx,endidx);
       moveCursor(rows,1);
       writeStat(strNormal);
    }

  }
}

void gotoDir(string str,bool stackpush){
  currentpath = str;
  if(stackpush){
    backStack.push(str);
    
  }
  listdir(str.c_str());
  endidx = nFiles;
  startidx=0;
  if(endidx > rows){
    endidx=rows;
  }
  linenum=0;
  printList(startidx,endidx);
  writeStat(strNormal);
}

void goUp(){
    currentpath.erase(currentpath.find_last_of('/'),string::npos);  
    
    gotoDir(currentpath,true);
}

void goBack(){
  string strback;
  if(!backStack.empty()){
    strback = backStack.top();
    backStack.pop();
    if(backStack.empty()){
      backStack.push(strback);
    }
    else{
      fwdStack.push(strback);
      gotoDir(backStack.top(),false);
    }

  }
  //writeStat(backStack.top().length()+"");
}

void goForward(){
  if(!fwdStack.empty()){
    string strfwd = fwdStack.top();
    fwdStack.pop();
    
    gotoDir(strfwd,true);
  }
}


