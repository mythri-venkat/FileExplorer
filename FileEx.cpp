

#include <termios.h>
#include <cstring>
#include <sys/ioctl.h>
#include"Events.h"
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
  normal_attr.c_lflag &= ~(ICANON|ECHO);
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
    }
  }
  cout << "\033[2J\033[1;1H";
  return 0;
}
