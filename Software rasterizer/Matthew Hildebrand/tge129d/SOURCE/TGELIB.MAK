#
# Use this makefile to create a copy of TGE.LIB which is usable by your
# compiler.
#
# By default, debugging information is not stored in the library.  To
# create a copy of TGE.LIB containing debugging information:
#    1. Edit the CC and ASM symbols to include the appropriate command-line
#       switches for your compiler and assembler, respectively.  To add or
#       remove debugging information to or from individual files, refer to
#       the end of this makefile.
#    2. Run MAKELIB.BAT to create a new copy of TGE.LIB (with debugging
#       information).
#
# By default, TGESETUP.C, CLIP.C, and MOUSEPTR.C are compiled as
# overlayable OBJ files.  With the exception of NEWMOUSE.ASM, any of TGE's
# modules may be overlaid; in general, however, only those modules which
# are used infrequently should be overlaid.  It is strongly recommended
# that TGE.C not be overlaid, except under the severest memory
# restrictions, as doing so may degrade performance.  To change the overlay
# status of one or more of TGE's modules, add or remove MAKE dependency and
# building information for the file(s) involved, following the examples at
# the end of this makefile.
#
# This makefile has only been tested with Borland C++ 2.0 to date, and
# assumes that a Borland compiler will be used; you may need to edit this
# file before using it with a non-Borland compiler.
#

.autodepend

MODEL       = l
CC_OVERLAY  = -Yo
ASM_OVERLAY = /o
CC_DEBUG    = -v
ASM_DEBUG   = /zi
CC          = bcc -c -m$(MODEL)
ASM         = tasm /ml
LIB         = tlib /c /e
OBJFILES    = tge.obj		\
              tgesetup.obj	\
              clip.obj		\
              tgefont.obj	\
              tgemouse.obj	\
              mouseptr.obj	\
              newmouse.obj	\
              scale.obj


.asm.obj:
     $(ASM) $<
.cpp.obj:
     $(CC) $<
.c.obj:
     $(CC) $<


tge.lib:  $(OBJFILES)
     $(LIB) ..\lib\tge @tgelib.lst


#
# To individually change a file's overlay or debugging information status,
# edit this section.  (Of course, the appropriate options may need to be
# passed to the linker for these options to take effect.)  Use the CC and
# CC_OVERLAY symbols for C/C++ source files, and the ASM and ASM_OVERLAY
# symbols for assembly source files.
#

tgesetup.obj:  tgesetup.c
     $(CC) $(CC_OVERLAY) tgesetup.c

clip.obj:  clip.c
     $(CC) $(CC_OVERLAY) clip.c

mouseptr.obj:  mouseptr.c
     $(CC) $(CC_OVERLAY) mouseptr.c

# Example of how to overlay an ASM file and include debugging information:
# tgemouse.obj:  tgemouse.asm
#      $(ASM) $(ASM_DEBUG) $(ASM_OVERLAY) tgemouse.asm
