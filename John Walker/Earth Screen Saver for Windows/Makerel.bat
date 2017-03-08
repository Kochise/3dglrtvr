rem
rem     Make release archive of Earth Screen Saver
rem
mfind temparch -rm -rmdir
mkdir temparch
copy release\earthscr.exe temparch\Earth.scr
copy readme.txt temparch\Readme.txt
del earthscr.zip
cd temparch
pkzip25 -add ..\earthscr.zip Earth.scr Readme.txt
cd ..
pkzip25 -test earthscr.zip
pkzip25 -view earthscr.zip
mfind temparch -rm -rmdir

