rem
rem	Make source archive of Earth Screen Saver
rem
mfind temparch -rm -rmdir
mkdir temparch
mkdir temparch\Debug
mkdir temparch\Release
copy *.bat temparch
copy *.bmp temparch
copy *.c   temparch
copy *.def temparch
copy *.vcproj temparch
copy *.sln temparch
copy *.h   temparch
copy *.ico temparch
copy *.rc  temparch
copy *.txt temparch
del earthscs.zip
cd temparch
pkzip25 -add -directories ..\earthscs.zip *.*
cd ..
pkzip25 -test earthscs.zip
pkzip25 -view earthscs.zip
mfind temparch -rm -rmdir
