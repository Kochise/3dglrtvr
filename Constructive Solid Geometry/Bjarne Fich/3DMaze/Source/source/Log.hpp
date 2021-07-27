#ifndef __LOG_HPP_
#define __LOG_HPP_

#include "Globals.hpp"

#ifndef DEBUG_MODE
#define LOG  ;//
#else
#define LOG  Log
#endif

#include <string>
using namespace std;

void Log(char *fmt, ...);
void Log_Exit(char *fmt, ...);
void ResetLog(string filename);
void LogIndexSet(int index);
void LogIndexAdd(int index);

#endif /* __LOG_HPP_ */

