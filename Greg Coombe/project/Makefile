#
# Makefile written by Greg Coombe
#
# Date: Thu Oct 29 01:52:10 MST 1998
#

.SUFFIXES: .o .cxx .cc .C .cpp .c
RM     = rm
OBJS   = GLModel.o Useful.o giwarp.o MatrixN.o illuminate.o
EXE    = project
GLVU_DIR = /afs/cs.unc.edu/project/hyre/common/we_demo/glvu

BASIC_LIBS = -L/afs/cs.unc.edu/project/walk/release/lib/sgi_irix.n32/ /afs/cs.unc.edu/project/walk/release/lib/sgi_irix/libglut.n32.a -lglut -lGL -lGLU -lXmu -lXext -lX11 -lm  


GLVU_CORE_LIBS = $(GLVU_DIR)/lib/libglvuglvu.a \
                 $(GLVU_DIR)/lib/libglvucamera.a \
                 $(GLVU_DIR)/lib/libglvumathobjs.a \
                 $(GLVU_DIR)/lib/libglvumath.a 



# SGI
#
# Very important note! Execute the command:
# unsetenv LD_LIBRARY_PATH
# to get rid of "soname be.so unable to link under ..."
#
LIBS	= ${GLVU_CORE_LIBS} ${BASIC_LIBS} 
INCLUDES = -I${GLVU_DIR}/math/  \
	   -I${GLVU_DIR}/glvu/ \
	   -I${GLVU_DIR}/object/objmodel/ \
           -I${GLVU_DIR}/camera/ \
           -I${GLVU_DIR}/object/ \
           -I/afs/cs.unc.edu/project/walk/release/include/ \
           -I/usr/local/contrib/unmod/include/ \
	   -I/afs/cs.unc.edu/home/coombe/include/

CFLAGS = -g -O2 ${INCLUDES}
CCC    = CC

# Sun
#LIBS	= -lX11 -lXext -lXi -lXmu -lglut -lGLU -lGL -lX11 -lm
#CFLAGS = -g -D DEBUG
#CCC    = g++

# Linux
#CCC    = g++
#CFLAGS = -g -D DEBUG
#INCLUDE   = -I${MY_INC} -I/usr/X11R6/include/ -I/usr/src/linux-2.2.16/include/
#LIBS  = -L/usr/X11R6/lib -lGLU -lGL -lglut -lX11 -lXext -lXi -lXmu -lm

# Now define all of our flags
# CFLAGS = -g -O -Wall
# Use -pg for profiling with gprof
# Use -O{1,2,3} to optimize


${EXE}: ${OBJS}
	${CCC} ${CFLAGS} -o ${EXE} ${OBJS} ${LIBS}

run: ${EXE}
	clear
	${EXE}

tar:
	tar -cvf ${EXE}.tar *.c* *.h* *.c *.h Makefile
	gzip ${EXE}.tar

depend:
	makedepend -Y *.c*

.o .cxx.o .cc.o .C.o .cpp.o .c.o:
	${CCC} ${CFLAGS} ${INCLUDE} -c $<

clean:
	${RM} *.o ${EXE} *~
# DO NOT DELETE

GLModel.o: GLModel.h Useful.h MatrixN.h Train.h GSample.h
GLModel.o: GLModel.h Useful.h MatrixN.h Train.h GSample.h
MatrixN.o: MatrixN.h Useful.h Train.h MElementPool.h
MatrixN.o: MatrixN.h Useful.h Train.h
Useful.o: Useful.h
giwarp.o: illuminate.h GLModel.h Useful.h MatrixN.h Train.h GSample.h
giwarp.o: ColorCycler.h
illuminate.o: illuminate.h GLModel.h Useful.h MatrixN.h Train.h GSample.h
