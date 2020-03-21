    #include "mpi.h"
    #include <stdio.h>

    #define DATOS "datos.dat"
    #define MAX_FILE 1024


    #define NORTE 0
    #define SUR 1
    #define ESTE 2
    #define OESTE 3

    MPI_Request request;
    MPI_Status status;


int enviarVecinos(int lado, double minimo, double bufferNumero, int *vecinos){
    int a;
    for(a=0; a<lado; a++){
        MPI_Isend(&minimo, 1, MPI_DOUBLE, vecinos[ESTE], 0, MPI_COMM_WORLD, &request);
        MPI_Recv(&bufferNumero, 1, MPI_DOUBLE, vecinos[OESTE], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Wait(&request, &status);

        if (bufferNumero < minimo)
        {  
            minimo = bufferNumero;
        }
    }
                

    int b;        
    for(b=0; b<lado; b++){
    MPI_Isend(&minimo, 1, MPI_DOUBLE, vecinos[NORTE], 0, MPI_COMM_WORLD, &request);
    MPI_Recv(&bufferNumero, 1, MPI_DOUBLE, vecinos[SUR], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Wait(&request, &status);

    if (bufferNumero < minimo)
    {  
        minimo = bufferNumero;
    }
    }
    return minimo;
}

    void obtenerVecinos(int *vecinos, int rank, int lado){


        int c = rank % lado;
        int f = rank / lado;
        if (c == 0)
        {
            vecinos[ESTE] = rank + (lado - 1);
        }
        else
        {
            vecinos[ESTE] = rank - 1;
        }
        if (f == 0)
        {
            vecinos[NORTE] = rank + ((lado - 1) * lado);
        }
        else
        {
            vecinos[NORTE] = rank - lado;
        }
        if (c == lado - 1)
        {
            vecinos[OESTE] = rank - (lado - 1);
        }
        else
        {
            vecinos[OESTE] = rank + 1;
        }
        if (f == lado - 1)
        {
            vecinos[SUR] = rank - (f * lado);
        }
        else
        {
            vecinos[SUR] = rank + lado;
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