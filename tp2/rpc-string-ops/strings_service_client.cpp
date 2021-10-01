#include <iostream>
#include "./strings_service.h"

#define LwS "LwS"
#define LWS "LWS"
#define NW "NW"

#define SERVER "localhost"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Uso: client <function -> [LwS (length with spaces) | LWS (length without spaces) | NW (number of words)]> <string>" << endl;
		exit(EXIT_FAILURE);
	}

	char *str = argv[2];
    string function = string(argv[1]);
    int* result = 0;

	CLIENT *clnt = clnt_create(SERVER, STRINGS_SERVICE, STRINGS_SERVICE_VER, "tcp");
	if (clnt == NULL) {
		cerr << "Error creando el cliente rpc..." << endl;
		clnt_pcreateerror(SERVER);
		exit(EXIT_FAILURE);
	}

	if(function == LwS) {
		cout << "procesando longitud con espacios de: " << str << endl;
		result = num_char_s_1(&str, clnt);

		if (result != NULL) {
			cout << "Resultado: " << *result << endl;
		}
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, SERVER);
		}
	}
    else if(function == LWS) {
        cout << "procesando longitud sin espacios de: " << str << endl;
		result = num_char_ws_1(&str, clnt);

		if (result != NULL) {
			cout << "Resultado: " << *result << endl;
		}
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, SERVER);
		}
    }
    else if(function == NW) {
        cout << "procesando cantidad de palabras de: " << str << endl;
		result = num_words_1(&str, clnt);

		if (result != NULL) {
			cout << "Resultado: " << *result << endl;
		}
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, SERVER);
		}
    }
	else {
		cerr << "Uso: client <function -> [LwS (length with spaces) | LWS (length without spaces) | NW (number of words)]> <string>" << endl;
		exit(EXIT_SUCCESS);
	}
	
	exit(EXIT_SUCCESS);
}