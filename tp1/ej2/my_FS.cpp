#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    if(argc == 5) {
        // argumentos del programa
        char const *server = argv[1], *file = argv[3], *local_dir = argv[4];
        int port = atoi(argv[2]);
        
        // variables para la conexion
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char buffer[1024] = {0};

        // creando socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("\n error creando socket\n");
            exit(EXIT_FAILURE);
        }

        // configuracion de la conexion
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // 
        if (inet_pton(AF_INET, server, &serv_addr.sin_addr) <= 0) {
            perror("\nDireccion invalida\n");
            exit(EXIT_FAILURE);
        }

        // conectandose al servidor
        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("\nerror en connect\n");
            exit(EXIT_FAILURE);
        }

        // enviando nombre del archivo
        send(sock, file, strlen(file), 0);

        // reciviendo archivo desde el servidor
        valread = read(sock, buffer, 1024);
        printf("\n- server: %s (%d)\n", buffer, valread);
        
        exit(EXIT_SUCCESS);
    }
    else {
        printf("uso: myFS <servidor> <puerto> <archivo> <directorio local>\n");
        exit(EXIT_FAILURE);
    }
}