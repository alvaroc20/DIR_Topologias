    #include "mpi.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "funciones.c"

    #define OK 0
    #define ERROR 1

    #define NORTE 0
    #define SUR 1
    #define ESTE 2
    #define OESTE 3

    int leerFichero(double *numeros);
    void obtenerVecinos(int *vecinos, int rank, int lado);
    int enviarVecinos(int lado, double minimo, double bufferNumero, int *vecinos);


    int main(int argc, char *argv[]){

        MPI_Status status;

        double minimo;
        int rank, size, estado;
        double bufferNumero;
        int lado = atoi(argv[1]);
        int vecinos[4];


        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
        MPI_Comm_size(MPI_COMM_WORLD, &size); 
        
        if (rank == 0)
        {
            int cantidadNumeros;
            double vector[MAX_FILE];
            cantidadNumeros = leerFichero(vector);

            if(size != lado*lado){
                fprintf(stderr, "*****Error*****\nSe esperaban %d procesos para un toroide de lado %d.\nUsted tiene %d procesos\n", lado*lado, lado, size);
                estado = ERROR;
                MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }else{
                if (cantidadNumeros != size)
                {
                    fprintf(stderr, "*****Error*****\nEl fichero contiene %d valores y se necesitan %d.\n",cantidadNumeros, size);
                    estado = ERROR;
                    MPI_Bcast(&estado, 1, MPI_INT, 0, MPI_COMM_WORLD);
                }else{
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
                obtenerVecinos(vecinos, rank, lado);
                printf("RANK[%d]. Norte: %d\t Este: %d\t Sur: %d\t, Oeste: %d \n", rank, vecinos[NORTE], vecinos[ESTE], vecinos[SUR], vecinos[OESTE]);


                minimo = bufferNumero;
                minimo = enviarVecinos(lado, minimo, bufferNumero, vecinos);
                
                if (rank == 0){
                    printf("RANK[%d].El numero minimo es %f\n",rank, minimo);
                }
            }
          

            MPI_Finalize();
            return EXIT_SUCCESS;
    }



