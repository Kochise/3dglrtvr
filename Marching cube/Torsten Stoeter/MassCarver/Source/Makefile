CC = g++
CCFLAGS = -O3 -funroll-loops -ffast-math -fomit-frame-pointer
LDFLAGS = -lglut -lGLEW
FILES = RawVolume.cpp Volume.cpp VolumeRenderer.cpp ColorScale.cpp Main.cpp

carve: $(FILES)
	$(CC) $(CCFLAGS) -o carve $(FILES) $(LDFLAGS)

clean:
	rm carve

