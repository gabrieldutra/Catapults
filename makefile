CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL
TODOS_ARQUIVOS_PONTOH = model/catapulta.h model/props.h
TODOS_ARQUIVOS_OBJ = catapults.o model/props.o model/catapulta.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o catapults.out $^ $(OPCOES_COMPILADOR)

run: all
	./catapults.out

clean:
	rm *.o model/*.o *.out
