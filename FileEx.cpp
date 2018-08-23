

#include"FileUtils.h"
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <sys/ioctl.h>
using namespace std;


struct termios default_attr;
struct winsize s;

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
    
    
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&s);
    char homepath[PATH_MAX];
    if(getcwd(homepath,sizeof(homepath))==NULL){
    	perror("getcwd error");
    	return 1;
    }
    //strcat(homepath,"\\TestDir");
    listdir(".");
    setNormalMode();
    char ch;
    int line=0;
    while((ch = getchar()) != 'q'){
    if ( ch == '\033') { 
    	getchar(); 
    	switch(getchar()) {
        	case 'A':
            	cout << "\033[1A";
            	line--;
            	break;
        	case 'B':
            	cout << "\033[1B";
            	line++;
            	break;
        	case 'C':
            	
            	break;
        	case 'D':
            	
            	break;
    	}
	}
	if(ch == '\n'){
		//cout << "line:"<<line;
		enterDir(line);
		line=0;
	}
	
}
cout << "\033[2J\033[1;1H";

    return 0;
}
