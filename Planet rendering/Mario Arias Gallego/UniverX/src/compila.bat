gcc -o univerx.exe geometria.c orbit.c main.c texturas.c texto.c stars.c shader.c -Wall -lopengl32 -lglu32 -lglut32 -lpng -ljpeg -lm -DMINGW32 -lz -mwindows -Wl,--no-whole-archive -lcg -lcgGL
gcc -o univerx_dbg.exe geometria.c orbit.c main.c texturas.c texto.c stars.c shader.c -Wall -lopengl32 -lglu32 -lglut32 -lpng -ljpeg -lm -DMINGW32 -lz -Wl,--no-whole-archive -lcg -lcgGL
univerx_dbg.exe
