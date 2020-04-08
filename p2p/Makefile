

all : test_server

test_server : test_server.o common.o
	gcc -o test_server test_server.o common.o -pthread

test_server.o : test_server.c
	gcc -c -o test_server.o test_server.c

common.o : common.c
	gcc -c -o common.o common.c

clean :
	rm -rf *.o test_server

