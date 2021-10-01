#include "./calculator.h"

double* sum_1_svc(Operands* operands, struct svc_req* req) {
	static double result;	
	result = operands->operand1 + operands->operand2;	
	return &result;
}

double* subtract_1_svc(Operands* operands, struct svc_req* req) {
	static double result;	
	result = operands->operand1 - operands->operand2;	
	return &result;
}

double* multiply_1_svc(Operands* operands, struct svc_req* req) {
	static double result;	
	result = operands->operand1 * operands->operand2;	
	return &result;
}

double* divide_1_svc(Operands* operands, struct svc_req* req) {
	static double result;	
	result = operands->operand1 / operands->operand2;	
	return &result;
}

char** equals_1_svc(Operands* operands, struct svc_req* req) {
	char *true = "true";
	char *false = "false";

	return operands->operand1 == operands->operand2 ? &true : &false;
}
