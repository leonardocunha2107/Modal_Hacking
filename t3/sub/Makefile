all: client server

client: client1.c
	gcc -o client client1.c

server: udp_server.c
	gcc -pthread -o server udp_server.c

clean:
	rm client server