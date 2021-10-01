all:
	g++ -Wall client.cpp calculator_clnt.c calculator_xdr.c -o client
	gcc -Wall calculator_ops.c calculator_svc.c calculator_xdr.c -o server

clean:
	rm -f client server