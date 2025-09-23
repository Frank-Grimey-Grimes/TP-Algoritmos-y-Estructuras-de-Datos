#ifndef TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED
#define TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED

#include <stdio.h>
//para la funcion rand()
#include <stdlib.h>
//para memcpy y funciones de string
#include <string.h>

//para "sembrar" la funcion rand()
#include <time.h>

/// MACROS
//ERRORES
#define TODO_OK 0
#define ERROR_MEMORIA 1
#define ARCHIVO_INEXISTENTE 2
//opciones para el menu
#define JUGAR 3
#define LEADERBOARD 4
#define SALIR 5
//estados del jugador
#define VIVO 18
#define MUERTO 13
#define VICTORIA 77

#define NOMARCHCONFIG "config.txt"
#define NOMARCHLAB "laberinto.txt"


typedef struct
{
    unsigned filas;
    unsigned columnas;
    unsigned vidasInicio;
    unsigned maxNumFantasmas;
    unsigned maxNumPremios;
    unsigned maxVidasExtra;
}config;

typedef struct
{
    int x;
    int y;
}parInt;


typedef struct
{
    int x;
    int y;
    int estado;
    int puntuacion;
}Jugador;

//menu
int menuOpciones();

///Juego
//asumiendo que el archivo de config.txt será un csv
int leerArchivoConfig(const char *nomArch, config *configuracion);
void cargarConfig(char *vec, config *configuracion);
int partida(config *configuracion);

// laberinto
int generarLaberinto(config *conf, unsigned *y, char*** laberinto);
int DFSgenerarCamino(config* conf, char** laberinto,unsigned x,unsigned y);
void mezclarMovimientos(parInt *vec);

//jugador
void iniciarJugador(Jugador *jug, unsigned y,char*** laberinto);
void movimientoJugador(char*** laberinto, Jugador *jug);

///Leaderboard

#endif // TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED
