tasm /ml %1
tlink /x %1,%1;
exe2bin %1.exe %1.drv
del %1.obj
del %1.exe