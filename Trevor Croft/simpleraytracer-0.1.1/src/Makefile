#Makefile

#----------------------------------------------------------
# after editing run 'make depend' before using.
#----------------------------------------------------------

#----------------------------------------------------------
# Edit the following variables
#------------------------------------------------------------
EXE	= simplert
SRCS	= main.cpp Shape.cpp Color.cpp Sphere.cpp Triangle.cpp Light.cpp Vector.cpp Ray.cpp RayTracer.cpp Point.cpp unit_limiter.cpp
#------------------------------------------------------------

#----------------------------------------------------------
# Edit the following compiler options if you wish
#----------------------------------------------------------
CCFLAGS = -Wall -Wstrict-prototypes -Wmissing-prototypes -g
CC 	= g++ $(CCFLAGS)
#----------------------------------------------------------

#----------------------------------------------------------
# Usages
#----------------------------------------------------------
# make depend - adds the dependencies (header files) to the build
#             - run make depend after the initial edit of this file
#             - and when any new header files are added to the project
#
# make clean  - removes all unnecessary files, including backup files *~
#             - make clean when you think old object files may be causing 
#	      - errors.
#
# make edit   - loads emacs with all the srcs (.cpp files) and all headers *.h
#
# make        - compile!
#----------------------------------------------------------

#----------------------------------------------------------
# Don't touch anything below here.
#----------------------------------------------------------
OBJS	= $(SRCS:.cpp=.o)

all: $(EXE)

%.o:%.cpp
	$(CC) -c $<

$(EXE): $(OBJS)
	$(CC) -o $@ $^

.PHONY: depend
depend: 
	makedepend -Y $(SRCS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXE) core *~ *#


.PHONEY: edit
edit:
	emacs $(SRCS) *.h &










# DO NOT DELETE
