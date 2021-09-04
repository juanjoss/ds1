#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

// c++
#include <map>
#include <fstream>

#define PORT 8080
#define DIR "./files/"

int main(int argc, char const *argv[]) {
    int sock_fd, new_sock, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = "";

    // cache
    std::map<std::string, std::string> cache;

    // creando socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("error creando socket");
        exit(EXIT_FAILURE);
    }

    // configuracion de la conexion
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

    // escuchando por peticiones
    printf("escuchando en puerto: %d, directorio de archivos: %s\n", PORT, DIR);

    while(1) {
        if ((new_sock = accept(sock_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("error en accept");
            continue;
        }

        printf("\n\n- new request: %s\n", inet_ntoa(address.sin_addr)); // IP del cliente

        // leyendo el nombre del archivo desde el cliente
        valread = read(new_sock, buffer, 1024);
        printf("- file requested: %s (%d)\n", buffer, valread);

        if(cache.find(buffer) == cache.end()) {
            printf("archivo %s no encontrado en cache\n", buffer);

            // TODO: controlar espacio de cache

            // abiendo archivo solicitado y enviando el contenido
            char path[] = DIR;
            strcat(path, buffer);

            FILE *f = fopen(path, "r");
            if(f != NULL) {
                fseek(f, 0, SEEK_END);
                long fsize = ftell(f);
                fseek(f, 0, SEEK_SET);

                char *content = (char *) malloc(fsize + 1);
                fread(content, 1, fsize, f);
                fclose(f);

                content[fsize] = 0;

                cache.insert({buffer, content});
                
                if(send(new_sock, content, strlen(content), 0) == -1) {
                    perror("error en send (archivo NO en cache)");
                }
            }
            else {
                // archivo no encontrado
                if(send(new_sock, "archivo no encontrado", 21, 0) == -1) {
                    perror("error en send (archivo NO en cache)");
                }
            }
        }
        else {
            printf("- archivo encontrado en cache");

            std::string file_content = cache.at(buffer);

            if(send(new_sock, file_content.c_str(), file_content.size(), 0) == -1) {
                perror("error en send (archivo en cache)");
            }
        }
    }

    exit(EXIT_SUCCESS);
}
