#ifndef INC_COMMANDMODE_H
#define INC_COMMANDMODE_H
#include "Events.h"

extern string command;

bool makeCommand(char c);
bool parseCommand(string command);

#endif