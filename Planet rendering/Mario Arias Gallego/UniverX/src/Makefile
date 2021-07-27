# UniverX Makefile

# Compiler and linker
CC=gcc

# Flags for compiler and linker
ifeq (Darwin,$(shell uname -s))
	CFLAGS= -I../../libpng/libpng-1.2.12 -I../../jpeg-6b -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch ppc -arch i386
	LDFLAGS = -framework Cocoa -framework OpenGL -framework GLUT -framework Foundation -framework libjpeg -framework libpng -framework Cg -lstdc++
else
	LDFLAGS = -lGL -lglut -lGLU -lm -ljpeg -lpng -lCg -lCgGL -lpthread
endif

# Code

all:	univerx

mac:	univerx
	cp univerx ../mac/bundle/UniverX.app/Contents/MacOS/univerx

univerx:	texto.o orbit.o texturas.o stars.o geometria.o main.o shader.o 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o univerx

texto.o:	texto.c texto.h constantes.h
	$(CC) $(CFLAGS) -c texto.c

orbit.o:	orbit.c orbit.h constantes.h
	$(CC) $(CFLAGS) -c orbit.c

texturas.o:	texturas.c texturas.h constantes.h
	$(CC) $(CFLAGS) -c texturas.c

stars.o:	stars.c stars.h constantes.h star_data.h shader.h
	$(CC) $(CFLAGS) -c stars.c

geometria.o:	geometria.c geometria.h constantes.h
	$(CC) $(CFLAGS) -c geometria.c

main.o:	main.c constantes.h texto.h orbit.h texturas.h stars.h geometria.h
	$(CC) $(CFLAGS) -c main.c

shader.o:	shader.c shader.h
	$(CC) $(CFLAGS) -c shader.c

clean:
	rm -f *.o *.a
	rm -f univerx
	rm -f *~
	rm -f *.exe

