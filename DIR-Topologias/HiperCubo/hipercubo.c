#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OK 0
#define ERROR 1

#define MAX_FILE 1024
#define DATOS "datos.dat"

MPI_Status status;
MPI_Request request;

int leerFichero(double *numeros);
void obtenerVecinos(int *vecinos, int rank, int dimension);
void obtenerMaximo(int rank, double bufferNumero, int *vecinos, int dimension, double *maximo);

int main(int argc, char *argv[]){

    int rank, size, estado;
    double bufferNumero;
    double numeroMaximo;
    int dimension = atoi(argv[1]);
    int N = (int) round(pow(2,dimension));
    int vecinos[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){

        int cantidadNumeros;
        double vector[MAX_FILE];
        cantidadNumeros = leerFichero(vector);

        if(size != N){
            fprintf(stderr, "*****Error*****\nSe esperaban %d procesos para un toroide de dimension %d.\nUsted tiene %d procesos\n", N, dimension, size);
            estado = ERROR;
            MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }else{
            if (cantidadNumeros != size){
                fprintf(stderr, "*****Error*****\nEl fichero contiene %d valores y se necesitan %d.\n",cantidadNumeros, N);
                estado = ERROR;
                MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
                    
                    int j;
                    for(j=0; j<cantidadNumeros; j++){
                        bufferNumero = vector[j];
                        MPI_Send(&bufferNumero, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
                    }
            }
            
        }

    }

    MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(estado == OK){
        MPI_Recv(&bufferNumero, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        obtenerVecinos(vecinos, rank, N);
       obtenerMaximo(rank, bufferNumero, vecinos, dimension, &numeroMaximo);


       if(rank == 0){
        printf("RANK[%d]: El numero maximo es %f\n", rank, numeroMaximo);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}


 void obtenerMaximo(int rank, double bufferNumero, int *vecinos, int dimension, double *maximo){

    int i;
    *maximo = bufferNumero;
    double recibido;

    for(i=0;i<dimension;i++){

		//Si el numero actual es mayor que el maximo
		if(recibido> *maximo){
			*maximo=recibido;

		}
        MPI_Isend(maximo, 1, MPI_DOUBLE, vecinos[i], 0, MPI_COMM_WORLD, &request);
        MPI_Recv(&recibido, 1, MPI_DOUBLE, vecinos[i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Wait(&request, &status);
        if(recibido>*maximo){
			*maximo=recibido;
		}

		}
}


void obtenerVecinos(int *vecinos, int rank, int dimension){
    int i;
    for(i = 0; i < dimension; i++){
        vecinos[i] = rank ^ (int) pow(2,i); 
    } 
}


int leerFichero(double *numeros){

        //Vector auxiliar de char para trabajar con los numeros del fichero
        char *listaNumeros=malloc(MAX_FILE * sizeof(char));

        //Tamanio del vector de numeros
        int cantidadNumeros=0;

        //Caracter auxiliar para trabajar con los numeros del fichero
        char *numeroActual;

        //Abrimos el fichero con permisos de lectura
        FILE *fichero=fopen(DATOS, "r");
        if(!fichero){
            fprintf(stderr,"*****Error*****: no se pudo abrir el fichero\n.");
            return 0;
        }
        //Copiamos los datos del fichero al vector auxiliar de char
        fscanf(fichero, "%s", listaNumeros);

        //Cerramos el fichero
        fclose(fichero);

        //Leemos el primer numero hasta la primera coma. Usamos la funcion strtok. Con atof transformamos el string a double
        numeros[cantidadNumeros++]=atof(strtok(listaNumeros,","));

        //Vamos leyendo hasta que no haya mas numeros delante de las comas
        while( (numeroActual = strtok(NULL, "," )) != NULL ){
            //Metemos en el vector el numero correspondiente
            numeros[cantidadNumeros++]=atof(numeroActual);
        }

        free(listaNumeros);
        return cantidadNumeros;
    }