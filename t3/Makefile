all: client1 server client2

client1: client1.c
	gcc -o client1 client1.c

server: udp_server.c
	cc -g -o server udp_server.c -lpthread

client2: client2.c
	gcc -o client2 client2.c

clean:
	rm client1 server client2