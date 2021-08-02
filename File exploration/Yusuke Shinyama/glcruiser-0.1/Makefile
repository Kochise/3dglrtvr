PROJ= xcruiser
CC= gcc
CFLAGS= -O2 -Wall -std=gnu99
LDLIBS= -lglut -lGLU -lGL

test: $(PROJ)
	./$(PROJ)

$(PROJ): screen.o draw.o zone.o vec3.o info.o xcruiser.o

clean:
	$(RM) *.o $(PROJ)

screen.o: xcruiser.h screen.h screen.c
draw.o: xcruiser.h screen.h draw.c
zone.o: xcruiser.h zone.c
info.o: xcruiser.h screen.h info.c
