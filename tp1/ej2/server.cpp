#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

// c++
#include <map>
#include <iostream>

#define PORT 8080
#define DIR "./files/"
#define LIM_CACHE 536870912

// cache
std::map<std::string, std::string> cache;
std::map<std::string, int> content_sizes;
int cache_size = 0;

int main(int argc, char const *argv[]) {
    int sock_fd, new_sock, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

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
        char buffer[1024] = "";

        if ((new_sock = accept(sock_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("error en accept");
            continue;
        }

        printf("\n\n- new request: %s\n", inet_ntoa(address.sin_addr)); // IP del cliente

        // leyendo el nombre del archivo desde el cliente
        valread = read(new_sock, &buffer, sizeof(buffer));
        printf("- file requested: %s (%d)\n", buffer, valread);

        if(cache.find(buffer) == cache.end()) {
            std::cout << "- archivo " << buffer << " no encontrado en cache" << std::endl;

            // abiendo archivo solicitado y enviando el contenido
            std::string path = DIR;
            path = path + buffer;

            FILE *f = fopen(path.c_str(), "r");
            if(f != NULL) {
                fseek(f, 0, SEEK_END);
                long fsize = ftell(f);
                fseek(f, 0, SEEK_SET);

                // tamaño de archivo > tamaño cache
                if(fsize > LIM_CACHE) {
                    char *content = (char *) malloc(fsize + 1);
                    fread(content, 1, fsize, f);

                    content[fsize] = 0;

                    std::cout << "- archivo demasiado grande para almacenar, enviando desde disco" << std::endl;
                    if(send(new_sock, content, strlen(content), 0) == -1) {
                        perror("error en send (archivo NO en cache)");
                    }
                }
                else {
                    // removiendo archivos de la cache (de mayor a menor tamaño)
                    while(cache_size + fsize > LIM_CACHE) {
                        std::string max;
                        int max_size = 0;
                        for (auto const x : content_sizes) {
                            if(x.second > max_size) {
                                max_size = x.second;
                                max = x.first;
                            }
                        }

                        std::cout << "- eliminando de cache archivo: " << max << std::endl;

                        cache_size -= max_size;
                        content_sizes.erase(max);
                        cache.erase(max);
                    }
                    
                    // el archivo entra en la cache
                    char *content = (char *) malloc(fsize + 1);
                    fread(content, 1, fsize, f);

                    content[fsize] = 0;
                    
                    // guardando a cache
                    cache[buffer] = content;
                    content_sizes[buffer] = fsize;
                    cache_size += fsize;
                    
                    std::cout << "- archivo almacenado en cache" << std::endl;
                    if(send(new_sock, content, strlen(content), 0) == -1) {
                        perror("error en send (archivo NO en cache)");
                    }
                }

                fclose(f);
                path = "";
            }
            else {
                // archivo no encontrado
                if(send(new_sock, "- archivo no encontrado", 21, 0) == -1) {
                    perror("error en send (archivo NO en cache)");
                }

                fclose(f);
            }
        }
        else {
            std::cout << "- archivo encontrado en cache" << std::endl;

            std::string file_content = cache.at(buffer);

            if(send(new_sock, file_content.c_str(), file_content.size(), 0) == -1) {
                perror("error en send (archivo en cache)");
            }
        }
    }

    exit(EXIT_SUCCESS);
}
