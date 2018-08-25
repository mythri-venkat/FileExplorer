#ifndef INC_EVENTS_H
#define INC_EVENTS_H

#include "FileUtils.h"

extern int startidx,endidx;
extern int linenum;
extern int rows;
void enterKey();
void upArrowKey();
void downArrowKey();
void gotoDir(string str,bool stackpush);
void goBack();
void goForward();
void goUp();
void writeStat(string str);
#endif