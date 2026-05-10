linux:
	cd obj && make
	gcc main.c -o uaz.exe $(shell pkg-config --cflags --libs sdl2) -Iobj/include/ obj/libobj.a -lGL -lGLU -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
windows:
	cd obj && make
	gcc main.c -o uaz.exe $(shell pkg-config --cflags --libs sdl2) -Iobj/include/ obj/libobj.a -lopengl32 -lglu32 -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
