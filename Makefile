all:
	gcc -pthread serverfolder/server.c -o serverfolder/server
	gcc c1/client.c -o c1/client
	gcc c2/client.c -o c2/client
	gcc c3/client.c -o c3/client
