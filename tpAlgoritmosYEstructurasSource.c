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
/* NOTA: Esta implementacion de generarLaberinto es muy simple y no logra generar laberintos
 con una complejidad suficiente para que el juego que sea jugable, pero puede permitirnos abstraernos de
 este problema en particular para encarar otras funciones como lo son el movimiento del jugador, puntaje, etc.*/
int generarLaberinto(config *conf)
{
    int i,j,columnaEntrada,columnaSalida;
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
        // columnas + 1 para guardar el caracter '\n', aunque no es necesario guardar este caracter en el txt
        // y guardarlo o no dependera de nuestra implementacion
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
    columnaEntrada = 1 + (rand()%(conf->columnas - 1));
    laberintoTemp[0][columnaEntrada] = 'E';
    //elegimos una poisicon al azar, desde 1 hasta columnas -1 en la ultima fila de la matriz para colocar la salida
    columnaSalida = 1 + (rand()%(conf->columnas - 1));
    laberintoTemp[conf->filas-1][columnaSalida] = 'S';

    //Generamos caminos aleatorios usando un pseudo DFS "deep finding search"
    DFSgenerarCamino(conf,laberintoTemp,1,columnaEntrada);
    // el algoritmo actual no crea un camino posible desde E hacia S, por lo que "abrimos" la celda directamente encima de S
    laberintoTemp[conf->filas-2][columnaSalida] = '.';

    //imprimimos laberinto para ver resultados. BORRAR LUEGO
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

int DFSgenerarCamino(config* conf, char** laberinto,unsigned x,unsigned y)
{
    int i,nx,ny;
    parInt movimientos[4];
    //marcamos la posicion actual como camino
    if(x != conf->filas-1 && y != conf->columnas-1)
    {
        laberinto[x][y] = '.';
        //  los movimientos circundantes a la posicion actual (IZQUIERDA, DERECHA, ARRIBA, ABAJO)
        //arriba
        movimientos[0].x = -1;
        movimientos[0].y = 0;
        //abajo
        movimientos[1].x = 1;
        movimientos[1].y = 0;
        //izquierda
        movimientos[2].x = 0;
        movimientos[2].y = -1;
        //derecha
        movimientos[3].x = 0;
        movimientos[3].y = 1;
        //ordenamos aleatoriamente el vector movimientos
        mezclarMovimientos(movimientos);
        for(i=0; i < 4;i++)
        {
            //nos movemos dos posiciones en la direccion del movimiento
            nx = x + movimientos[i].x * 2;
            ny = y + movimientos[i].y * 2;
            //si estamos dentro de la matriz y el caracter en esta posicion es '#' entramos
            //si no, aqui se rompe la recursividad
            if ( ( nx  > 0 && nx < conf->filas) && (ny >0 && ny < conf->columnas) && laberinto[nx][ny] == '#' )
            {
                //cambiamos el caracter de la posicion intermedia entre la actual y la anterior a '.'
                laberinto[x+movimientos[i].x][y+movimientos[i].y] = '.';
                //recursividad
                DFSgenerarCamino(conf,laberinto,nx,ny);
            }
        }
    }
    return TODO_OK;
}

void mezclarMovimientos(parInt *vec)
{
    int i,r;
    parInt aux;
    for(i = 0; i < 4;i++)
    {
        r = rand() % 4;
        aux = vec[r];
        vec[r] = vec[i];
        vec[i] = aux;
    }
}
