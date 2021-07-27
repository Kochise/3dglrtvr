#
# Use this makefile to create a copy of BCL.LIB (Borland C++ large model).
#
# You will need the registered version of TGE to use this makefile, as the
# shareware version does not include source code.
#
# By default, debugging information is not stored in the library.  To
# create a copy of BCL.LIB containing debugging information:
#    1. Edit the CC and ASM symbols to include the appropriate command-line
#       switches for your compiler and assembler, respectively.  To add or
#       remove debugging information to or from individual files, refer to
#       the end of this makefile.
#    2. Run 'MAKE -fbcl.mak' to create a new copy of BCL.LIB (with
#       debugging information).
#
# By default, TGESETUP.C, CLIP.C, and MOUSEPTR.C are compiled as
# overlayable OBJ files.  With the exception of NEWMOUSE.ASM, any of TGE's
# modules may be overlaid; in general, however, only those modules which
# are used infrequently should be overlaid.  It is strongly recommended
# that TGEFUNCS.C not be overlaid, except under the severest memory
# restrictions, as doing so may degrade performance.  To change the overlay
# status of one or more of TGE's modules, add or remove MAKE dependency and
# building information for the file(s) involved, following the examples at
# the end of this makefile.
#

.autodepend

CC_OVERLAY  = -Yo
ASM_OVERLAY = /o
CC_DEBUG    = -v
ASM_DEBUG   = /zi
CC          = bcc -c -ml -O -Z -d -r -G -a -2 -Vf- -V -k-
ASM         = tasm /ml
LIB         = tlib /c /e
OBJFILES    = ..\source\tgefuncs.obj	\
              ..\source\tgesetup.obj	\
              ..\source\clip.obj	\
              ..\source\fixfont.obj	\
              ..\source\varfont.obj	\
              ..\source\tgemouse.obj	\
              ..\source\mouseptr.obj	\
              ..\source\newmouse.obj	\
              ..\source\scale.obj	\
              ..\source\palette.obj	\
              ..\source\rawfile.obj	\
              ..\source\pcxfile.obj     \
              ..\source\replace.obj


.asm.obj:
     $(ASM) $<
.cpp.obj:
     $(CC) $<
.c.obj:
     $(CC) $<


bcl.lib:  $(OBJFILES)
     $(LIB) bcl.lib @tgelib.lst
     del *.obj

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
