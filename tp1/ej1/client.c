#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"

int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *msg = "Hola mundo cliente";
    char buffer[1024] = {0};

    // creando socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\n Socket creation error \n");
        return -1;
    }

    // configuracion de la conexion
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
        perror("\nDireccion invalida\n");
        exit(EXIT_FAILURE);
    }

    // conectandose al servidor
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("\nConexion fallida\n");
        exit(EXIT_FAILURE);
    }
    
    // enviando mensaje
    send(sock, msg, strlen(msg), 0);
    valread = read(sock, buffer, 1024);
    printf("- Server: %s (%d)\n", buffer, valread);

    exit(EXIT_SUCCESS);
}