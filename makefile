CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lm -ISDL -lSDL_mixer `sdl-config --cflags --libs` 
TODOS_ARQUIVOS_PONTOH = model/props/props.h model/balista/balista.h model/mapa/mapa.h model/tiro/tiro.h model/asteroide/asteroide.h model/barra/barra.h model/menu/menu.h
TODOS_ARQUIVOS_OBJ = catapults.o model/props/props.o model/mapa/mapa.o model/balista/balista.o model/tiro/tiro.o model/asteroide/asteroide.o model/barra/barra.o model/menu/menu.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o catapults.out $^ $(OPCOES_COMPILADOR)

run: all
	./catapults.out

clean:
	rm *.o model/*/*.o *.out

packages:
	sudo apt-get install freeglut3 freeglut3-dev libglew-dev libglew1.5-dev libglm-dev libsoil-dev libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
