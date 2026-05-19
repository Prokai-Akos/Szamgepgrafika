linux:
	cd includes/obj && make
	gcc main.c -o countrysideAdventure.exe $(shell pkg-config --cflags --libs sdl2) -Iincludes -I includes/obj/include -Iincludes/obj src/*.c includes/obj/libobj.a -lGL -lGLU -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
windows:
	cd includes/obj && make
	gcc main.c -o countrysideAdventure.exe $(shell pkg-config --cflags --libs sdl2) -Iincludes -Iincludes/obj/include -Iincludes/obj src/*.c includes/obj/libobj.a -lopengl32 -lglu32 -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
