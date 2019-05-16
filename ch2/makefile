all: flood

flood: flood.c header
	gcc -o flood flood.c *.o
	
header: header.h header.c
	gcc -c header.c