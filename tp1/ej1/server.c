#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock_fd, new_sock, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // creando socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("error creando socket");
        exit(EXIT_FAILURE);
    }

    // configurando la conexion
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("error en bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 3) < 0) {
        perror("error en listen");
        exit(EXIT_FAILURE);
    }

    if ((new_sock = accept(sock_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        perror("error en accept");
        exit(EXIT_FAILURE);
    }

    // leyendo mensaje del cliente
    valread = read(new_sock, buffer, 1024);
    printf("- Cliente: %s (%d)\n", buffer, valread);
    send(new_sock, buffer, strlen(buffer), 0); // echo al cliente

    exit(EXIT_SUCCESS);
}
