#ifndef INC_EVENTS_H
#define INC_EVENTS_H
#include <sys/ioctl.h>
#include "FileUtils.h"

extern int startidx, endidx;
extern int linenum;
extern int rows;
extern bool cmdMode;
extern int curPositionCmd;
void enterKey();
void upArrowKey();
void downArrowKey();
void gotoDir(string str, bool stackpush);
void goBack();
void goForward();
void goUp();
void writeStat(string str);
void writeStatCmd(string str);
void moveCursor(int x, int y);
#endif