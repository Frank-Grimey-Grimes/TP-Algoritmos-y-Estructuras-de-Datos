#include "tpAlgoritmosYEstructurasHeader.h"

// vuelca la informacion del archivo nomArch en un vector para ahorrar operaciones de lectura
// a desarrollar luego
int leerArchivoTexto(const char* nomArch, void* elem)
{
    unsigned tamArch;
    FILE *fp;
    fp = fopen(nomArch,"rt");
    if(fp == NULL)
        return ARCHIVO_INEXISTENTE;
    fseek(fp,0,SEEK_END);
    tamArch= ftell(fp);
    return TODO_OK;
}

//generará el laberinto en una matriz dinamica y luego lo volcará en el archvio .txt para ahorrar operaciones de escritura
int generarLaberinto(config *conf)
{
    int i,j;
    char** laberintoTemp;
    FILE *fp;
    // creamos el archivo "laberinto.txt"
    /*fp = fopen("laberinto.txt","wt");
    if (fp == NULL)
        return ARCHIVO_INEXISTENTE;*/

    // asignamos memoria a la matriz dinamica
    laberintoTemp = malloc(sizeof(char *) * (conf->filas));
    if (laberintoTemp == NULL)
        return ERROR_MEMORIA;

    for(i = 0; i < conf->filas; i++)
    {
        // columnas + 1 para guardar el caracter '\n'
        laberintoTemp[i] = malloc(sizeof(char) * (conf->columnas + 1));

        if(laberintoTemp[i] == NULL)
            return ERROR_MEMORIA;
    }

    // inicializamos la matriz llenando con #
    for(i = 0; i < conf->filas; i++)
    {
        for(j = 0; j < conf->columnas; j++)
        {
            laberintoTemp[i][j] = '#';
        }
        laberintoTemp[i][j] = '\n';
    }

    //sembramos rand()
    srand(time(NULL));
    // elegimos una posicion al azar, desde 1 hasta columnas - 1 en la primer fila de la matriz para colocar la entrada
    laberintoTemp[0][1 + (rand()%(conf->columnas - 1))] = 'E';
    //elegimos una poisicon al azar, desde 1 hasta columnas -1 en la ultima fila de la matriz para colocar la salida
    laberintoTemp[conf->filas-1][1 + (rand()%(conf->columnas - 1))] = 'S';

    //prueba
    for(i = 0; i < conf->filas; i++)
    {
        for(j = 0; j <= conf->columnas; j++)
        {
            printf("%c",laberintoTemp[i][j]);
        }
    }

    //liberamos la memoria utilizada para la matriz
    for(i = 0; i < conf->filas; i++)
    {
        free(laberintoTemp[i]);
    }
    free(laberintoTemp);

    return TODO_OK;
}
