cantidadNodos:=4
DIMENSION:=2
compilarHipercubo:
	mpicc -Wall -g hipercubo.c -o hipercubo -lm

ejecutarHipercubo:
	mpirun -n $(cantidadNodos) ./hipercubo $(DIMENSION)

all:	compilarHipercubo ejecutarHipercubo