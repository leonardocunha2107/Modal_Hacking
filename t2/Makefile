all: client server

client: udp_client.c
	gcc -o client udp_client.c

server: udp_server.c
	gcc -o server udp_server.c

clean:
	rm client server