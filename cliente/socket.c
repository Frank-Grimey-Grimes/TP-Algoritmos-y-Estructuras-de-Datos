#include "socket.h"
void testSocket_cliente() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char mensaje[1024];

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Error al inicializar Winsock: %d\n", WSAGetLastError());
        return;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Error al crear socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error al conectar con servidor\n");
        closesocket(s);
        WSACleanup();
        return;
    }

    printf("Conectado al servidor. Escriba comandos:\n");
    printf("Formato: OPCION MENSAJE\n");
    printf("OPCION: MAYUS / MINUS / INV\n");

    while(1) {
        printf("> ");
        if (!fgets(mensaje, sizeof(mensaje), stdin)) break;
        mensaje[strcspn(mensaje, "\n")] = '\0'; // eliminar salto de línea

        if (strlen(mensaje) == 0) continue;

        send(s, mensaje, strlen(mensaje), 0);

        char buffer[1024];
        int recv_size = recv(s, buffer, sizeof(buffer)-1, 0);
        if (recv_size <= 0) break;
        buffer[recv_size] = '\0';

        printf("Servidor responde: %s\n", buffer);
    }

    closesocket(s);
    WSACleanup();
}
