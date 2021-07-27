#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <stdarg.h>

#include "Log.hpp"

char logfilename[256];
int logindex = 0;

void Log(char *fmt, ...) {
  va_list args;
  char buf[512];
  int i;

  ofstream fout(logfilename, ios::app);
  for (i=0; i< logindex; i++) fout << ' ';
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  fout << buf;
  fout << '\n';
  fout.close();
}

void Log_Exit(char *fmt, ...)
{
  va_list args;
  char buf[512];
  int i;

  ofstream fout(logfilename, ios::app);
  for (i=0; i< logindex; i++) fout << ' ';
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  fout << buf;
  fout << '\n';
  fout.close();

  ChangeDisplaySettings(NULL,0); // Full screen off
  exit(1);
}

void ResetLog(string filename) {
  tm *tmsp;
  time_t tp;
  int i;
  for (i=0; i<filename.size(); i++) logfilename[i] = filename[i];
  logfilename[i] = 0;
  LogIndexSet(0);
  ofstream fout(logfilename);
  fout.close();

  tp = time(NULL);
  tmsp = localtime(&tp);
  Log("Time: %s", asctime(tmsp));
}

void LogIndexSet(int index) {
  logindex = index;
}

void LogIndexAdd(int index) {
  logindex += index;
}

