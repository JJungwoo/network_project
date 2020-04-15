CC = gcc
CFLAGS = -pthread
TARGET = build/pear_agent


$(TARGET) : pear_agent.o common.o
	$(CC) $(CFLAGS) -o $(TARGET) build/pear_agent.o build/common.o

pear_agent.o : pear_agent.c
	$(CC) $(CFLAGS) -c -o build/pear_agent.o pear_agent.c

common.o : ../common.c
	$(CC) $(CFLAGS) -c -o build/common.o ../common.c

clean :
	rm -rf *.o $(TARGET) build/*.o

