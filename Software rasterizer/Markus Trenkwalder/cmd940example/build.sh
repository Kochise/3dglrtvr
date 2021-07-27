#!/bin/sh

# First compile the framework.
cd cmd940
./build.sh
cd ..

# Now compile test application.
arm-linux-gcc -Wall ./src/main.cpp -o test.gpe -L"./cmd940/lib/" -lstdc++ -lcmd940 -static 