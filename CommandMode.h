#ifndef INC_COMMANDMODE_H
#define INC_COMMANDMODE_H
#include "Events.h"

extern string command;
extern char homepath[PATH_MAX];
bool makeCommand(char c);
void parseCommand(string command);

#endif