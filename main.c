#include "tpAlgoritmosYEstructurasHeader.h"

/// tpAlgoritmosYEstructuras V1
int main()
{
    config configuracion;
    configuracion.filas = 12;
    configuracion.columnas = 12;
    generarLaberinto(&configuracion);
    return 0;
}
