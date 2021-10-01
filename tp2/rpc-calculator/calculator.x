struct Operands {
	double operand1;
	double operand2;
};

program CALCULATOR {
	version CALCULATOR_VER {
		double SUM(Operands) = 1;
		double SUBTRACT(Operands) = 2;
		double MULTIPLY(Operands) = 3;
		double DIVIDE(Operands) = 4;
		string EQUALS(Operands) = 5;
	} = 1;
} = 0x20000001;
