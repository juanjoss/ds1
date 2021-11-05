#include "./my_FS.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <mutex>

// c++
#include <map>
#include <iostream>

#define PORT 8080
#define DIR "./files/"
#define LIM_CACHE 536870912
#define MSG_NOT_FOUND "archivo no encontrado"

using namespace std;

// cache
map<string, string> cache;
map<string, int> content_sizes;
int cache_size = 0;

// mutex
sem_t s;

char **get_file_1_svc(char **filename, struct svc_req* req) {
    sem_init(&s, 0, 1);
    string fn = string(*filename);
    static char *content = NULL;

    if(cache.find(fn) == cache.end()) {
        cout << "- archivo " << fn << " no encontrado en cache" << endl;

        // abiendo archivo solicitado y enviando el contenido
        string path = DIR;
        path = path + fn;

        FILE *f = fopen(path.c_str(), "r");
        if(f != NULL) {
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);

            // tamaño de archivo > tamaño cache
            if(fsize > LIM_CACHE) {
                content = (char *) malloc(fsize + 1);
                fread(content, 1, fsize, f);

                content[fsize] = 0;

                cout << "- archivo demasiado grande para almacenar, enviando desde disco" << endl;
                return &content;
            }
            else {
                // removiendo archivos de la cache (de mayor a menor tamaño)
                sem_wait(&s);
                while(cache_size + fsize > LIM_CACHE) {
                    string max;
                    int max_size = 0;
                    for (auto const x : content_sizes) {
                        if(x.second > max_size) {
                            max_size = x.second;
                            max = x.first;
                        }
                    }

                    cout << "- eliminando de cache archivo: " << max << endl;

                    cache_size -= max_size;
                    content_sizes.erase(max);
                    cache.erase(max);
                }
                sem_post(&s);
                
                // el archivo entra en la cache
                content = (char *) malloc(fsize + 1);
                fread(content, 1, fsize, f);

                content[fsize] = 0;
                
                // guardando a cache
                sem_wait(&s);
                cache[fn] = content;
                content_sizes[fn] = fsize;
                cache_size += fsize;
                sem_post(&s);
                
                cout << "- archivo almacenado en cache" << endl;
                
                return &content;
            }

            fclose(f);
            path = "";
        }
        else {
            // archivo no encontrado
            cout << "- archivo no encontrado" << endl;
            content = MSG_NOT_FOUND;

            return &content;
        }
    }
    else {
        cout << "- archivo encontrado en cache" << endl;

        sem_wait(&s);
        string file_content = cache.at(fn);
        strcpy(content, file_content.c_str());
        sem_post(&s);
        
        return &content;
    }

    sem_destroy(&s);
}
