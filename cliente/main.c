#include <stdio.h>
#include <stdlib.h>
#include "socket.h"
#include "juego.h"
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
                        break;
            case LEADERBOARD: break;
            default: break;
        }
    }

    //testSocket_cliente();
    return 0;
}
