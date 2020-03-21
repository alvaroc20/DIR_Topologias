cantidadNodos:=9
LADO:=3
compilarToroide:
	mpicc -Wall -g toroide.c -o toroide

ejecutarToroide:
	mpirun -n $(cantidadNodos) ./toroide $(LADO)

all:	compilarToroide ejecutarToroide