#include "./my_FS.h"

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
#define LIM_CACHE 524288
#define MSG_NOT_FOUND "archivo no encontrado"

using namespace std;

// cache
map<string, string> cache;
map<string, int> content_sizes;
int cache_size = 0;

char **get_file_1_svc(char **filename, struct svc_req* req) {
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
                
                // el archivo entra en la cache
                content = (char *) malloc(fsize + 1);
                fread(content, 1, fsize, f);

                content[fsize] = 0;
                
                // guardando a cache
                cache[fn] = content;
                content_sizes[fn] = fsize;
                cache_size += fsize;
                
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

        string file_content = cache.at(fn);
        strcpy(content, file_content.c_str());
        
        return &content;
    }
}
