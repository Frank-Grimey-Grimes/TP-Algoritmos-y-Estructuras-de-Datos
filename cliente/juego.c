#include "juego.h"

///Funciones Menu
int menuOpciones()
{
    char eleccion = 0;
    puts("BIENVENIDO! Elija una opcion");
    puts("1- Jugar\n2-Leaderboard\n3-Salir");
    while(eleccion!='1' && eleccion!='2' && eleccion!='3')
    {
        fflush(stdin);
        scanf("%c",&eleccion);
    }
    switch(eleccion)
    {
        case '1': return JUGAR;
        break;
        case '2': return LEADERBOARD;
        break;
        case '3': return SALIR;
        break;
    }
    return SALIR;
}

///Funciones Partida
int partida(config *configuracion)
{
    int i,j;
    char** laberinto;
    Jugador player;
    parInt **fantasmas;
    // y contiene el numero de la columna en la que se encuentra la salida, la usamos para ubicar al jugador en esa columna
    unsigned y;

    //generamos el laberinto que usaremos en la partida
    generarLaberinto(configuracion,&y,&laberinto);

    //iniciamos al jugador en la partida
    iniciarJugador(&player,y,&laberinto);
    system("cls");

    //insertamos las vidas extra
    generarVidasExtra(configuracion,laberinto);

    //insertamos los premios
    generarPremios(configuracion,laberinto);

    //insertamos los fantasmas
    if(generarFantasmas(configuracion,laberinto,&fantasmas) == ERROR_MEMORIA)
        return ERROR_MEMORIA;

    //mientras el jugador tenga vidas y no haya ganado
    while(player.vidas > 0 && player.estado != VICTORIA)
    {
        //mientras el jugador este vivo y el jugador no haya llegado al final
        while(player.estado == VIVO)
        {
            system("cls");
            for(i = 0; i < configuracion->filas; i++)
                {
                    for(j = 0; j <= configuracion->columnas; j++)
                    {
                        printf("%c",laberinto[i][j]);
                    }
                }
            movimientoJugador(&laberinto,&player);
            system("cls");
            for(i = 0; i < configuracion->filas; i++)
                {
                    for(j = 0; j <= configuracion->columnas; j++)
                    {
                        printf("%c",laberinto[i][j]);
                    }
                }
            if(player.estado != MUERTO)
            {
                movimientoFantasmas(&laberinto, &fantasmas, &player, configuracion);
            }
            system("cls");
            for(i = 0; i < configuracion->filas; i++)
                {
                    for(j = 0; j <= configuracion->columnas; j++)
                    {
                        printf("%c",laberinto[i][j]);
                    }
                }
        }
        //si el jugador se cruzó con un fantasma
        if(player.estado == MUERTO)
        {
            //le restamos una vida
            player.vidas--;
            //lo devolvemos a la entrada
            player.x = 1;
            player.y = y;
            laberinto[player.x][player.y] = 'J';
            player.estado = VIVO;
        }
    }

    printf("PUNTUACION: %d\n",player.puntuacion);

    //liberamos la memoria utilizada por el laberinto
    for(i = 0; i < configuracion->maxNumFantasmas; i++)
    {
        free(fantasmas[i]);
    }
    free(fantasmas);

    for( i = 0; i < configuracion->filas; i++)
    {
        free(laberinto[i]);
    }
    free(laberinto);
    return TODO_OK;
}

///Funciones Laberinto
// lee el archivo de configuracion
int leerArchivoConfig(const char *nomArch, config *configuracion)
{
    char *vec;
    unsigned tamArch;
    FILE *fp;
    fp = fopen(nomArch,"rt");
    if(fp == NULL)
        return ARCHIVO_INEXISTENTE;
    fseek(fp,0,SEEK_END);
    tamArch= ftell(fp);
    fseek(fp,0,SEEK_SET);
    vec = (char*)malloc(tamArch);
    if(vec == NULL)
    {
        return ERROR_MEMORIA;
    }
    // el primer fgets es para leer, y al mismo tiempo, saltear los encabezados del config.txt
    fgets(vec,tamArch,fp);
    /*
    en el segundo fgets, leemos los valores de las opciones de configuracion. Los leemos en vec
    para pisar la informacion que habiamos leido de los encabezados en el fgets anterior
    */
    fgets(vec,tamArch,fp);
    fclose(fp);
    //cargarConfig ira leyendo el vector en el que cargamos las opciones y cargando configuracion con dicha informacion
    cargarConfig(vec,configuracion);
    free(vec);
    return TODO_OK;
}

void cargarConfig(char *vec, config *configuracion)
{
    char *aux;
    /*
    strtok encuentra la primer instancia de ","  en el vector vec y la reemplaza con un '\0'
    permitiendonos "dividir" el vector en strings de informacion que enviaremos a atoi para convertir en numeros
    y guardar dichos numeros en los distintos campos de configuracion
    */
    aux = strtok(vec,",");
    configuracion->filas = atoi(aux);
    aux = strtok(NULL,",");
    configuracion->columnas = atoi(aux);
    aux = strtok(NULL,",");
    configuracion->vidasInicio = atoi(aux);
    aux = strtok(NULL,",");
    configuracion->maxNumFantasmas = atoi(aux);
    aux = strtok(NULL,",");
    configuracion->maxNumPremios = atoi(aux);
    aux = strtok(NULL,",");
    configuracion->maxVidasExtra = atoi(aux);
    return;
}

//generará el laberinto en una matriz dinamica y luego lo volcará en el archvio .txt para ahorrar operaciones de escritura
//Ademas, devuelve por parametro la columna en donde se encuentra la salida
/* NOTA: Esta implementacion de generarLaberinto es muy simple y no logra generar laberintos
 con una complejidad suficiente para que el juego que sea jugable, pero puede permitirnos abstraernos de
 este problema en particular para encarar otras funciones como lo son el movimiento del jugador, puntaje, etc.*/
int generarLaberinto(config *conf, unsigned *y, char*** laberinto)
{
    int i,j,columnaEntrada,columnaSalida;
    char** laberintoTemp;
    FILE *fp;
    // creamos el archivo "laberinto.txt"
    fp = fopen(NOMARCHLAB,"wt");
    if (fp == NULL)
        return ARCHIVO_INEXISTENTE;

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
    columnaEntrada = 1 + (rand()%(conf->columnas - 2));
    laberintoTemp[0][columnaEntrada] = 'E';

    //seteamos el parametro en el que devolveremos la columna de salida
    *y = columnaEntrada;

    //elegimos una poisicon al azar, desde 1 hasta columnas -1 en la ultima fila de la matriz para colocar la salida
    columnaSalida = 1 + (rand()%(conf->columnas - 2));
    laberintoTemp[conf->filas-1][columnaSalida] = 'S';

    //Generamos caminos aleatorios usando un pseudo DFS "deep finding search"
    DFSgenerarCamino(conf,laberintoTemp,1,columnaEntrada);
    // el algoritmo actual no crea un camino posible desde E hacia S, por lo que "abrimos" la celda directamente encima de S
    laberintoTemp[conf->filas-2][columnaSalida] = '.';

    //Escribimos el laberinto en el archivo "laberinto.txt"
    for(i = 0; i < conf->filas; i++)
    {
        fwrite(laberintoTemp[i], conf->columnas + 1, 1,fp);
    }

    // devolvemos la direccion del laberinto por parametro
    *laberinto = laberintoTemp;

    //cerramos el archivo
    fclose(fp);
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

int generarVidasExtra(config *conf, char** laberinto)
{
    int i,fila,columna;
    if(laberinto == NULL)
        return ERROR_MEMORIA;
    for(i = 0; i < conf->maxVidasExtra; i++)
    {
        srand(time(NULL));
        fila = 1 + (rand()%(conf->filas - 2));
        columna = 1 + (rand()%(conf->columnas - 2));
        while(laberinto[fila][columna] != '.' && laberinto[fila][columna] != '#')
              {
                  fila = 1 + (rand()%(conf->filas - 2));
                columna = 1 + (rand()%(conf->columnas - 2));
              }
        laberinto[fila][columna] = 'V';
    }
    return TODO_OK;
}

int generarPremios(config *conf, char** laberinto)
{
    int i,fila,columna;
    if(laberinto == NULL)
        return ERROR_MEMORIA;
    for(i = 0; i < conf->maxNumPremios; i++)
    {
        srand(time(NULL));
        fila = 1 + (rand()%(conf->filas - 2));
        columna = 1 + (rand()%(conf->columnas - 2));
        while(laberinto[fila][columna] != '.' && laberinto[fila][columna] != '#')
              {
                  fila = 1 + (rand()%(conf->filas - 2));
                columna = 1 + (rand()%(conf->columnas - 2));
              }
        laberinto[fila][columna] = 'P';
    }
    return TODO_OK;
}

//fantasmas es un vector dinamico de parInts que guarda la posicion de cada fantasma.
int generarFantasmas(config *conf, char** laberinto,parInt ***fantasmas)
{
    int i,j;
    parInt *ghost;
    *fantasmas = (parInt**)malloc(sizeof(parInt *) * conf->maxNumFantasmas);
    if(*fantasmas == NULL)
        return ERROR_MEMORIA;
    for(i = 0; i < conf->maxNumFantasmas; i++)
    {
        ghost = (parInt *)malloc(sizeof(parInt));
        if(ghost == NULL)
        {
            for(j = 0; j < i - 1; j++)
            {
                free((*fantasmas)[i]);
                free(*fantasmas);
                return ERROR_MEMORIA;
            }
        }
        ghost->x = 1 + (rand()%(conf->filas - 2));
        ghost->y = 1 + (rand()%(conf->columnas - 2));
        while(laberinto[ghost->x][ghost->y] != '.' && laberinto[ghost->x][ghost->y] != '#')
        {
            ghost->x = 1 + (rand()%(conf->filas - 2));
            ghost->y = 1 + (rand()%(conf->columnas - 2));
        }
        laberinto[ghost->x][ghost->y] = 'F';
        (*fantasmas)[i] = ghost;
    }

    return TODO_OK;
}

///Funciones jugador
void iniciarJugador(Jugador *jug, unsigned y,char*** laberinto)
{
    jug->puntuacion = 0;
    jug->vidas = 1;
    jug->estado = VIVO;
    jug->x = 1;
    jug->y = y;
    (*laberinto)[jug->x][jug->y] = 'J';
}

void movimientoJugador(char*** laberinto,Jugador *jug)
{
    char c;
    parInt movimiento, nuevaPos;
    fflush(stdin);
    c = getchar();
    switch(c)
    {
        case 'W':
        case 'w':
                    movimiento.x = -1;
                    movimiento.y = 0;
                  break;
        case 'S':
        case 's':
                    movimiento.x = 1;
                    movimiento.y = 0;
                  break;
        case 'A':
        case 'a':
                    movimiento.x = 0;
                    movimiento.y = -1;
                  break;
        case 'D':
        case 'd':
                    movimiento.x = 0;
                    movimiento.y = 1;
                  break;
        default: return;
    }
    nuevaPos.x = jug->x + movimiento.x;
    nuevaPos.y = jug->y + movimiento.y;
    if((*laberinto)[nuevaPos.x][nuevaPos.y] != '#' && (*laberinto)[nuevaPos.x][nuevaPos.y] != 'S'
       && (*laberinto)[nuevaPos.x][nuevaPos.y] != 'E')
    {
        //si la nueva posicion del jugador es la de un fantasma
        if((*laberinto)[nuevaPos.x][nuevaPos.y] == 'F')
        {
            jug->estado = MUERTO;
            (*laberinto)[jug->x][jug->y] = '.';
            return;
        }
        // si la nueva posicion del jugador es la de un premio
        if((*laberinto)[nuevaPos.x][nuevaPos.y] == 'P')
            jug->puntuacion++;

        //si la nueva posicion del jugador es la de una vida extra
        if((*laberinto)[nuevaPos.x][nuevaPos.y] == 'V')
            jug->vidas++;
        (*laberinto)[nuevaPos.x][nuevaPos.y] = 'J';
        (*laberinto)[jug->x][jug->y] = '.';
        jug->x = nuevaPos.x;
        jug->y = nuevaPos.y;
    }
    else if((*laberinto)[nuevaPos.x][nuevaPos.y] == 'S')
    {
        jug->puntuacion += 1000;
        jug->estado = VICTORIA;
    }
    return;
}

///Funciones Fantasmas
// esta logica de movimiento para los fantasmas elige una direccion al azar de las direcciones en las que no
// haya una pared, premio o vida extra para que el fantasma se mueva
// no es muy interesante ni muy divertida
void movimientoFantasmas(char*** laberinto, parInt ***fantasmas, Jugador *jug, config *conf)
{
    int i,j,nx,ny,cantValidos;
    parInt movimientos[4] = {
        { -1, 0 },  // arriba
        { 1, 0 },   // abajo
        { 0, -1 },  // izquierda
        { 0, 1 }    // derecha
    };
    parInt movValidos[4];

    for(i = 0; i < conf->maxNumFantasmas; i++)
    {
        cantValidos = 0;
        for(j = 0; j < 4; j++)
        {
            nx = (*fantasmas)[i]->x + movimientos[j].x;
            ny = (*fantasmas)[i]->y + movimientos[j].y;

            if((*laberinto)[nx][ny] == '.' || (*laberinto)[nx][ny] == 'J')
            {
                movValidos[cantValidos] = movimientos[j];
                cantValidos++;
            }
        }

        if(cantValidos > 0)
        {
            srand(time(NULL));
            j = rand() % cantValidos;
            //si el proximo movimiento corresponde a la posicion del jugador
            if((*laberinto)[(*fantasmas)[i]->x + movValidos[j].x][(*fantasmas)[i]->y + movValidos[j].y] == 'J')
            {
                //cambiar el estado del jugador a "MUERTO"
                jug->estado = MUERTO;
            }
            //borramos vieja posicion
           (*laberinto)[(*fantasmas)[i]->x][(*fantasmas)[i]->y] = '.';
            //actualizamos posicion
            (*fantasmas)[i]->x += movValidos[j].x;
            (*fantasmas)[i]->y += movValidos[j].y;
            //dibujamos nueva posicion
            (*laberinto)[(*fantasmas)[i]->x][(*fantasmas)[i]->y] = 'F';
        }
    }
}
