all:
	g++ -Wall my_FS_client.cpp my_FS_clnt.c -o client
	g++ my_FS_ops.cpp my_FS_svc.c -Wall -lpthread -lrt -o server

clean:
	rm -f client server