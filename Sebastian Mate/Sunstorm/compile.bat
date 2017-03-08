@echo off
BC /E/X/O/AH/T/C:512 SUNSTORM.bas;
LINK /EX /NOE SUNSTORM.obj,,nul,+dqb.lib+ffix.lib;
