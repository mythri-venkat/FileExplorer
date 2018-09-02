

#include <termios.h>
#include <cstring>

#include "Events.h"
#include "CommandMode.h"

using namespace std;

struct termios default_attr;

//reset default settings.
void resetMode()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &default_attr);
}

//set non canonical mode.
void setNormalMode()
{
  struct termios normal_attr;

  if (!isatty(STDIN_FILENO))
  {
    cout << "Not Terminal";
    exit(EXIT_FAILURE);
  }

  tcgetattr(STDIN_FILENO, &default_attr);
  atexit(resetMode);
  tcgetattr(STDIN_FILENO, &normal_attr);
  normal_attr.c_lflag &= ~(ICANON | ECHO | ECHOE);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal_attr);
}

int main()
{
  //get window height.
  struct winsize s;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &s);
  rows = --s.ws_row;

  //get current working directory and make it home.i.e. application root.
  if (getcwd(homepath, sizeof(homepath)) == NULL)
  {
    perror("getcwd error");
    return 1;
  }
  //print contents of home.
  gotoDir(homepath, true);
  writeStat("-- NORMAL MODE --");
  //set to non canonincal input mode.
  setNormalMode();
  char ch;
  //wait for user action in normal mode.
  while ((ch = getchar()) != 'q')
  {
    switch (ch)
    {
    case '\033':
    {
      getchar();
      switch (getchar())
      {
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
    case '\n':
    {
      enterKey();
      break;
    }
    case 'h':
    case 'H':
    {
      gotoDir(homepath, true);
      break;
    }
    case ((char)127):
    case '\b':
    {
      goUp();
      break;
    }
    case ':':
    { 
      //enter command mode on press of : and exit on esc.
      cmdMode = true;
      curPositionCmd = 2;
      writeStatCmd("");
      while (1)
      {
        char ch1 = getchar();
        if (makeCommand(ch1))
        {
          curPositionCmd = 1;
          writeStat("--NORMAL MODE--");
          moveCursor(1, 1);
          linenum = startidx;
          command = "";
          break;
        }
      }
      cmdMode = false;
      break;
    }
    }
  }
  cout << "\033[2J\033[1;1H";
  return 0;
}
