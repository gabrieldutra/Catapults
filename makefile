CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lm
TODOS_ARQUIVOS_PONTOH = model/balista.h model/props.h
TODOS_ARQUIVOS_OBJ = catapults.o model/props.o model/balista.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o catapults.out $^ $(OPCOES_COMPILADOR)

run: all
	./catapults.out

clean:
	rm *.o model/*.o *.out
