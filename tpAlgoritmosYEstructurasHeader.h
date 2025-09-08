#ifndef TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED
#define TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED

#include <stdio.h>
//para la funcion rand()
#include <stdlib.h>

//para "sembrar" la funcion rand()
#include <time.h>

#define TODO_OK 0
#define ERROR_MEMORIA 1
#define ARCHIVO_INEXISTENTE 2

typedef struct
{
    unsigned filas;
    unsigned columnas;
    unsigned vidasInicio;
    unsigned maxNumFantasmas;
    unsigned maxNumPremios;
    unsigned maxVidasExtra;
}config;

int leerArchivoTexto(const char*, void* elem);
int generarLaberinto(config *conf);

#endif // TPALGORITMOSYESTRUCTURASHEADER_H_INCLUDED
