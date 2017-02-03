#
# Use this makefile to create TGEDEMO.EXE with your compiler.  This
# makefile assumes that TGE.LIB is on your system; if it isn't, use
# ..\LIB\MAKELIB.BAT to build it.
#
# This makefile has only been tested with Borland C++ 2.0 to date, and
# assumes that a Borland compiler will be used; you may need to edit this
# file before using it with a non-Borland compiler.
#

.autodepend

MODEL       = l
CC          = bcc -c -m$(MODEL)
LINK        = tlink /x /c /yx
LINK_DEBUG  = /v
DEPENDFILES = tgedemo.obj	\
              ..\lib\tge.lib


.asm.obj:
        $(ASM) $<
.cpp.obj:
        $(CC) $<
.c.obj:
        $(CC) $<


tgedemo.exe:  $(DEPENDFILES)
     $(LINK) c0l.obj tgedemo.obj, tgedemo.exe,, cl.lib ..\lib\tge.lib
