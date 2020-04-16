CC = gcc
CFLAGS = -pthread
TARGET = pear_agent

COMMON_DIR = ../common
COMMON_SRC = $(COMMON_DIR)/common.c

$(TARGET) : pear_agent.o common.o
	$(CC) $(CFLAGS) -o $(TARGET) build/pear_agent.o build/common.o

pear_agent.o : pear_agent.c
	$(CC) $(CFLAGS) -c -o build/pear_agent.o pear_agent.c

common.o : $(COMMON_SRC)
	$(CC) $(CFLAGS) -c -o build/common.o $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o

