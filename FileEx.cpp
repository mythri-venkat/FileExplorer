

#include <termios.h>
#include <cstring>
#include <sys/ioctl.h>
#include"Events.h"
#include "CommandMode.h"

using namespace std;


struct termios default_attr;




void resetMode (){
  tcsetattr (STDIN_FILENO, TCSANOW, &default_attr);
}

void setNormalMode (){
  struct termios normal_attr;

  if (!isatty (STDIN_FILENO))
    {
      cout << "Not Terminal";
      exit (EXIT_FAILURE);
    }

  tcgetattr (STDIN_FILENO, &default_attr);
  atexit (resetMode);
  tcgetattr (STDIN_FILENO, &normal_attr);
  normal_attr.c_lflag &= ~(ICANON|ECHO|ECHOE);
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &normal_attr);
}



int main() {

    struct winsize s;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&s);
    rows=--s.ws_row;
    char homepath[PATH_MAX];
    if(getcwd(homepath,sizeof(homepath))==NULL){
    	perror("getcwd error");
    	return 1;
    }
    //strcat(homepath,"\\TestDir");
    gotoDir(homepath,true);
    writeStat("--NORMAL MODE--");
    setNormalMode();
    char ch;
    
    while((ch = getchar()) != 'q'){
      switch(ch){
        case'\033': { 
    	  getchar(); 
    	  switch(getchar()) {
        	case 'A':
              upArrowKey();
            	break;
        	case 'B':
              downArrowKey();
            	break;
        	case 'C':
            	goForward();
            	break;
        	case 'D':
              goBack();
            	break;
    	  }
        break;
	    }
      case '\n':{
       enterKey();
       break;		
	    }
      case 'h':
      case 'H':{
        gotoDir(homepath,true);
        break;
      }
      case ((char)127):
      case '\b':{
        goUp();
        break;
      }
      case ':':{
        resetMode();
        writeStat(":");
        moveCursor(rows+1,2);
        
        char chcmdmode=':';
        while(1){
          
          if(chcmdmode == ':'){
            //resetMode();
            string command;
            getline(cin,command);
            parseCommand(command);
            //setNormalMode();
          }
          else if(chcmdmode == '\e'){
            moveCursor(1,1);
            break;
          }
          //chcmdmode = getchar();          
        }       
      }
      
    }
  }
  cout << "\033[2J\033[1;1H";
  return 0;
}
