
CC = gcc
CFLAGS = -pthread
TARGET = packet_test

COMMON_DIR = ../common
COMMON_SRC = $(COMMON_DIR)/common.c

INC_DIRS = -I$(COMMON_DIR)/

$(TARGET) : packet_test.o common,o
	$(CC) $(CFLAGS) $(INC_DIRS) -o $(TARGET) build/packet_test.o build/common.o

packet_test.o : packet_test.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/packet_test.o packet_test.c

common.o : common.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/common.o $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o 
