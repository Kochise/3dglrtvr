cls
@echo.
@echo    Building Gens2.EXE ...
@echo.
@echo off

cd source
if exist *.pas copy *.pas ..\compiler\bin
if exist *.inc copy *.inc ..\compiler\bin
if exist *.res copy *.res ..\compiler\bin
if exist GMain.cfg copy GMain.cfg ..\compiler\bin

cd ..\compiler\bin
if exist GMain.pas TMTPC GMain.pas

if exist *.fpw del *.fpw
if exist *.fsm del *.fsm
if exist *.pas del *.pas
if exist *.inc del *.inc
if exist *.res del *.res
if exist GMain.cfg del GMain.cfg

if not exist GMain.exe goto error
if exist GMain.exe ren GMain.exe Gens2.exe
if exist Gens2.exe copy Gens2.exe ..\..\Bin
if exist Gens2.exe del Gens2.exe
cd ..\..

@echo.
@echo    Building complete. You can find the Gens2.EXE in the BIN subdirectory.
@echo.
goto end

:error
cd ..\..
@echo.
@echo    Error building EXE  !!!
@echo.

:End
