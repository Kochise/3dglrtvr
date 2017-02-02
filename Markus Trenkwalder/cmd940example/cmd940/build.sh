#!/bin/sh

# Generate sources
cd src
lua gencmd.lua

# Compile everything for the 940
arm-linux-gcc -O3 -Wall -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fomit-frame-pointer -c -DARM940 *.cpp
mv *.o ../bin
cd cmd940
arm-linux-gcc -O3 -Wall -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fomit-frame-pointer -c -DARM940 *.cpp
mv *.o ../../bin
cd ../../bin

# Link everything
arm-linux-ld.exe -o code940.gpe -Ttext 0x0 -T ../link940.lds *.o
arm-linux-objcopy.exe -O binary code940.gpe code940.bin

# Remove unneeded stuff
rm *.gpe *.o

# Compile everything for the 920 and make a library
cd ../src/cmd920
arm-linux-gcc -O3 -Wall -fomit-frame-pointer -c *.cpp
mv *.o ../../lib
cd ../../lib
arm-linux-ar rcs libcmd940.a *.o
rm *.o
