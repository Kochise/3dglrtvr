@echo off

REM Dos-Script for creating lightmaps using LMTools.
REM ------------------------------------------------
REM 
REM Usage:
REM  
REM   lmtools.bat base_name [q.s. for lmgen] [q.s. for lmpack]
REM
REM (q.s = quality settings)
REM

REM First generate lightmaps
lmgen.exe %1 %2
if errorlevel = 1 goto error01

REM Now, pack them!
lmpack.exe %1 %3
if errorlevel = 1 goto error02

REM Alright! :)
goto theend

:error01
ECHO.
ECHO ERROR while CREATING lightmaps.
goto theend

:error02
ECHO.
ECHO ERROR while PACKING lightmaps.
goto theend

:theend