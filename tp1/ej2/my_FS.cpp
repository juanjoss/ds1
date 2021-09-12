#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int recv_timeout(int s, int timeout, const char *local_dir);

int main(int argc, char const *argv[]) {
    if(argc == 5) {
        // argumentos del programa
        char const *server = argv[1], *file = argv[3], *local_dir = argv[4];
        int port = atoi(argv[2]);
        
        // variables para la conexion
        int sock = 0;
        struct sockaddr_in serv_addr;

        // creando socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("\nerror creando socket\n");
            exit(EXIT_FAILURE);
        }

        // configuracion de la conexion
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // 
        if (inet_pton(AF_INET, server, &serv_addr.sin_addr) <= 0) {
            perror("\ndireccion invalida\n");
            exit(EXIT_FAILURE);
        }

        // conectandose al servidor
        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("\nerror en connect\n");
            exit(EXIT_FAILURE);
        }

        // enviando nombre del archivo
        send(sock, file, strlen(file), 0);

        // recibiendo archivo desde el servidor
        int total_recv = recv_timeout(sock, 1, local_dir);

        printf("- contenido recibido del servidor: %d bytes\n", total_recv);
        
        exit(EXIT_SUCCESS);
    }
    else {
        printf("uso: myFS <servidor> <puerto> <archivo> <directorio local>\n");
        exit(EXIT_FAILURE);
    }
}

/*
    Recibir datos en varios bloques chequeando un socket no bloqueante
    con un timeout y guardar a la ruta especificada por local_dir.
*/
int recv_timeout(int s, int timeout, const char *local_dir) {
	int size_recv, total_size = 0;
	struct timeval begin, now;
	char chunk[BUFFER_SIZE];
	double timediff;
	
	// pasa el socket a no bloqueante
	fcntl(s, F_SETFL, O_NONBLOCK);
	
	// tiempo de inicio
	gettimeofday(&begin , NULL);

    // abriendo el archivo de almacenamiento destino
    FILE *f = fopen(local_dir, "w");
    if(f == NULL) {
        printf("- la ruta de almacenamiento destino es incorrecta");
        exit(EXIT_FAILURE);
    }
	
	while(1) {
		gettimeofday(&now , NULL);
		
		// tiempo transcurrido en segundos
		timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
		
        // si se obtuvo informacion, entonces cortar luego del timeout
		if(total_size > 0 && timediff > timeout) {
			break;
		}
        // si no se recibio informacion, esperar el doble que el timeout 
		else if(timediff > 2*timeout) {
			break;
		}
		
		memset(chunk, 0, BUFFER_SIZE); // vaciar la variable
		if((size_recv =  recv(s, chunk, BUFFER_SIZE, 0)) < 0) {
            // si no se recibio nada, se espera 0.1 segundos antes de intentar nuevamente
			usleep(100000);
		}
		else {
			total_size += size_recv;
            fwrite(chunk, sizeof(char), strlen(chunk), f);
            
			// reinicio del tiempo de inicio
			gettimeofday(&begin, NULL);
		}
	}

    // cerrando el archivo
    fclose(f);
	
	return total_size;
}