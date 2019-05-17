all: switch
	gcc -o switch *.o

switch: switch.c header
	gcc -c switch.c
	
header: header.h header.c
	gcc -c header.c
test: test.c 
	gcc -o test test.c 

clean: 
	rm *.o switch