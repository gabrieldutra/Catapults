CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lm
TODOS_ARQUIVOS_PONTOH = model/props/props.h model/balista/balista.h model/mapa/mapa.h model/tiro/tiro.h model/asteroide/asteroide.h
TODOS_ARQUIVOS_OBJ = catapults.o model/props/props.o model/mapa/mapa.o model/balista/balista.o model/tiro/tiro.o model/asteroide/asteroide.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o catapults.out $^ $(OPCOES_COMPILADOR)

run: all
	./catapults.out

clean:
	rm *.o model/*/*.o *.out
