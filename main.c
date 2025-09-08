#include "tpAlgoritmosYEstructurasHeader.h"

int main()
{
    config configuracion;
    configuracion.filas = 15;
    configuracion.columnas = 15;
    generarLaberinto(&configuracion);
    return 0;
}
