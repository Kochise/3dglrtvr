@echo off
setlocal

echo Extracting png files...
rem http://www.swftools.org/download.html
set "path=%path%;c:\Program Files (x86)\SWFTools;"
"python" extract.py "..\xcruiser-demo.swf"

echo Deleting useless png files...
rem There are several useless mouse cursor pictures
del files.txt 2>nul
for /f "usebackq delims=;" %%a in (`dir /odn /b /s *.png`) do if %%~za lss 2048 (del "%%a") else (echo "%%a">>files.txt)

echo Creating animated gif file...
rem https://imagemagick.org/script/download.php
rem Windows' convert.exe is used to convert disk partition
"c:\Program Files\ImageMagick\convert.exe" -delay 10 -loop 0 @files.txt xcruiser-demo.gif
del files.txt 2>nul
