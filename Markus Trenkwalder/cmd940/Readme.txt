This is the cmd940 framework.


Intro:
--------------------------------------------------------------------------------

This is a framework for the GP2X which will make it easier to utilize the 
ARM940t CPU. The framework consists of a set of source files and scripts and is 
supposed to be extended to suit each individual need.

Using this framework you basically are able to call a function from the 920 and 
it will automatically be executed on the 940. It may also have an optional 
return value.

So it is as simple as the following function call to execute stuff on the 940.

    unsigned int result = cmd940::factorial(10);

Naturally the function has to be implemented on the 940.

The above function will be executed synchronously as it has a return value but
that must not necessarily be the case. Consider the following example:

    // This is async as it has no return value that we would have to wait for
    cmd940::fibonacci_async(35);

    // ... do some work here ...
    
    // Now get the result back. This is synchronous and will wait until the
    // actual computation is finished.
    unsigned int result = cmd940::fibonacci_async_last_result();


Memory Layout:
--------------------------------------------------------------------------------

The framework uses the first megabyte of upper memory for the program and the 
stack. The second megabyte is used for the two ringbuffers. So you have all the
upper memory above the first two megabytes at your disposal. If this is 
undesireable this can easily be changed in the sources.


Compiling:
--------------------------------------------------------------------------------

Visual Studio solution and project files are provided that use the rule file 
from the gp2x wiki (there is one small modification from the file on the wiki
as explained in a note about the ARM940). The rule file works for the 
devkitGP2X. You still need the make make_940_bin.bat available from the wiki 
though. 

If you can't or don't want to use Visual Studio you have to write your own 
makefile or equivalent or adapt the provided build script.

All the source files in src/cmd920 have to be compiled together to form the 
lib/libcmd940.a file.

All the source files from src/cmd940 (without the src/cmd940/cmds directory) 
together with the files in src/ have to be compiled and liked together to form
bin/code940.bin.


Workflow:
--------------------------------------------------------------------------------

There are some files that need to be modified those will be listed below and an
explanation will be given.

1. Edit src/cmdlist.lua

   In this file you list all your functions including their signature that you 
   want to be able to call from the 920.

2. Run the src/gencmd.lua script

   This script will take the cmdlist.lua file which now should contain the 
   function definitions and does some things with it.

   - It will generate src/cmd940/cmds directory and put one .cpp file for each 
     function in the list in this directory. If a file already exists it won't 
     be overwritten. Those functions are still missing their implementation 
     which you are supposed to supply by editing the files.

   - It will generate src/cmd940/cmds940.cpp which will contain the stub 
     functions that parse the function arguments from a ringbuffer and execute 
     your defined functions.

   - It will generate src/cmd920/cmds920.cpp. This file contain the stub 
     functions for the 920. They take the function parameters and put it into 
     the ringbuffer which will then be read by the 940.

   - It will generate include/interface.h which contains the function 
     declarations for the interface on the 920.

3. Edit include/user_include.h file

   This file will be included for the 920 and the 940 before the command 
   declarations. This file should define all the types needed in the command 
   interface and may also include other files (e.g. if you use struct types and
   such as the function parameters).

4. Edit src/cmd940/cmds940.cpp.h file

   This file will be included before the command implemenatation files and is
   supposed to define all the necessary symbols needed for the implementation
   of the commands.

5. You are now ready to compile everything.

   A library file libcmd940.a will be generated in the lib/ directory which is
   the library file you can use for your projects. It conaints the functions you
   defined.

   The file code940.bin is generated in the bin/ directory. This file containts
   the implementation for the 940 and a path to this file has to be passed to 
   the library's init function. It will execute this on the 940.


Limitations when programming for the ARM940T
--------------------------------------------------------------------------------

There are a few limitations when developing for the 940. Some may be specific to
devkitGP2X, which I use, and some may apply to any devkit.

- No runtime library
  
  When developing for the 940 you don't have any runtime library available and 
  you can't link to the same as you would for the 920. Still trying to might 
  cause problems. There is no automatic startup code generated.

- No dynamic division

  While you can have integer division and modulo in the form variable/constant
  you can't have arbitrary divisions as the linker will complain about not 
  finding some symbols. While those sysbols are defined in some libgcc.a file
  and linking to that solved the "undefined reference" problems the code still 
  didn't work (There where some "swi" instructions which is not good on the 
  940).

- No floats

  Using floats will make the linker complain about undefined references to 
  floating point related functions.
  
- Problems with classes having destructors or virtual functions

  Having classes with virtual functions will create undefined references to the
  vtable. Using -fno-rtti can solve this. This is also a good idea to do on the 
  940 I think.
  Using classes with destructors will cause multiple undefined references (e.g.
  to __cxa_atexit) when using global variables. Classes with destructors 
  instantiated on the stack could also cause problems. I had undefined referenes
  to some unwind functions. They were probably related to exception stuff as I
  don't have this problem now.

  The cmd940 framework provides some dummy functions to overcome these problems.

- Constructors of global/static instances of classes won't get called.

  Although having a global variable of a class with a constructor compiles fine
  the startup code which is supposed to call the constructors won't get 
  executed thus the variables won't be initialized. Also undefined global 
  variables wouldn't get initialized to the value 0 as is required.

  The cmd940 framework comes some with support routines which overcome this 
  limitation. Uninitialized global variables will get initialized to 0 and the
  constructors of global class instances will get called.

- Linking problems

  You need to make sure that the entry point function ends up at address 0 in 
  your binary for the 940. That means the object file containing the entry point
  function needs to be the first one passed to the linker ld. With devkitGP2X on 
  Windows it seems one can ensure this by prepending a $ to the source file 
  containing the entry point as $ has a low ASCII value and it seems the object 
  files are passed in alphabetical order.
  The entry point function also needs to be the first function defined in the
  source file. You can't have any other functions implemented before this.
  It seems to be possible to have function implementations before the entry
  point when they are declared inline (and only inline) but I discourage relying 
  on this. All this is specific to devkitGP2X on Windows. On other toolkits or
  OSes it might behave diffenertly and may require your own linker script.

  Therefore cmd940 now comes with its own linker script. It ensures that the 
  start940.o object file will be the first one to be linked and therefore the
  address of the entry point will be 0.

- Compiling options

  It seems a good idea to use -nostdlib -fno-builtin -fno-rtti -fno-exceptions
  for C++ projects on the 940. It seems to work without -fno-exceptions but if
  you don't use them it's better to disable them.


Credits
--------------------------------------------------------------------------------

- Main programmer and copyright holder: Markus Trenkwalder a.k.a Trenki

- Many thanks to hlide for helping me integrate the runtime support 
  (initializing global variables and calling constructors).

- Many thanks to the people on #gp2xdev irc channel for helping me find some 
  bugs.


