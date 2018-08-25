
#include"Events.h"
using namespace std;

int startidx=0,endidx=0;
int linenum=0;
int rows;

void writeStat(string str){
  
}

void moveCursor(int x,int y){
  cout << "\033["<<x<<";"<<y<<"H";
}

void enterKey(){
  FileType type = enterDir(linenum);
  if(type == Dir){
    startidx =0;
    endidx = nFiles;
    if(endidx > rows){
      endidx=rows;
    }
    printList(startidx,endidx);
	  linenum=0;
  }
}

void upArrowKey(){
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
    }
  }
}

void downArrowKey(){
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
  if(endidx > rows){
    endidx=rows;
  }
  printList(startidx,endidx);
  
}

void goUp(){
    gotoDir("..",true);
    string strback = backStack.top();
    backStack.pop();
    fwdStack.push(strback);
}

void goBack(){
  if(!backStack.empty()){
    string strback = backStack.top();
    backStack.pop();
    if(backStack.empty()){
      backStack.push(strback);
    }
    else{
      fwdStack.push(strback);
      gotoDir(backStack.top(),false);
    }

  }
}

void goForward(){
  if(!fwdStack.empty()){
    string strfwd = fwdStack.top();
    fwdStack.pop();
    
    gotoDir(strfwd,true);
  }
}


