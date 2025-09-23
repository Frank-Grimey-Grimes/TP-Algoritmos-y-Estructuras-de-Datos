#include "tpAlgoritmosYEstructurasHeader.h"

/// tpAlgoritmosYEstructuras V1
int main()
{
    //pre juego
    int estadoDelJuego;
    config configuracion;
    leerArchivoConfig(NOMARCHCONFIG,&configuracion);
    //juego
    estadoDelJuego = 0;
    while(estadoDelJuego!=SALIR)
    {
        estadoDelJuego = menuOpciones();
        switch(estadoDelJuego)
        {
            case JUGAR: partida(&configuracion);
            case LEADERBOARD: break;
            default: break;
        }
    }


    return 0;
}
