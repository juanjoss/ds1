#include <iostream>
#include "./calculator.h"

#define _SUM "+"
#define _SUBTRACT "-"
#define _MULTIPLY "x"
#define _DIVIDE "/"
#define _EQUALS "="

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cerr << "Uso: client <host> <op1> <operation -> [+ | - | x | / | =]> <op2>" << endl;
		exit(EXIT_FAILURE);
	}
	char *server = argv[1];
	string operation = string(argv[3]);

	CLIENT *clnt = clnt_create(server, CALCULATOR, CALCULATOR_VER, "tcp");
	if (clnt == NULL) {
		cerr << "Error creando el cliente rpc..." << endl;
		clnt_pcreateerror(server);
		exit(EXIT_FAILURE);
	}

	Operands op;
	try {
		op.operand1 = stod(argv[2]);
		op.operand2 = stod(argv[4]);
	}
	catch(const std::exception& e) {
		cerr << "Operandos invalidos" << endl;
		exit(EXIT_FAILURE);
	}
	
	double* result;
	char** comparation;

	if(operation == _SUM) {
		cout << "Procesando: " << op.operand1 << operation << op.operand2 << endl;
		result = sum_1(&op, clnt);

		if (result != NULL) {
			cout << *result << endl;
		}
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, server);
		}
	}
	else if(operation == _SUBTRACT) {
		cout << "Procesando: " << op.operand1 << operation << op.operand2 << endl;
		result = subtract_1(&op, clnt);

		if (result != NULL) {
			cout << *result << endl;
		}
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, server);
		}
	}
	else if(operation == _MULTIPLY) {
		cout << "Procesando: " << op.operand1 << operation << op.operand2 << endl;
		result = multiply_1(&op, clnt);

		if (result != NULL) {
			cout << *result << endl;
		} 
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, server);
		}
	}
	else if(operation == _DIVIDE) {
		cout << "Procesando: " << op.operand1 << operation << op.operand2 << endl;
		result = divide_1(&op, clnt);

		if (result != NULL) {
			cout << *result << endl;
		} 
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, server);
		}
	}
	else if(operation == _EQUALS) {
		cout << "Procesando: " << op.operand1 << operation << op.operand2 << endl;
		comparation = equals_1(&op, clnt);

		if (comparation != NULL) {
			cout << *comparation << endl;
		} 
		else {
			cerr << "error" << endl;
			clnt_perror(clnt, server);
		}
	}
	else {
		cout << "Uso: client <host> <op1> <operation -> [+ | - | x | / | =]> <op2>" << endl;
		exit(EXIT_SUCCESS);
	}
	
	exit(EXIT_SUCCESS);
}