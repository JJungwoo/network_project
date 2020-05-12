CC = gcc
CFLAGS = -pthread
TARGET = ftp_server 

SRC = ftp_server.c  

BUILD = build/
OUTPUT = $(BUILD)/ftp_server.o $(BUILD)/common.o

COMMON_DIR = ../common
COMMON_SRC = $(COMMON_DIR)/common.c

INC_DIRS = $(COMMON)

TARGET_OBJS = $(TARGET_SRCS:%.c=$(OBJS_DIR)/%.o)




$(TARGET) : pear_agent.o common.o
	$(CC) $(CFLAGS) $(INC_DIRS) -o $(TARGET) build/pear_agent.o build/common.o

pear_agent.o : pear_agent.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o  pear_agent.c

common.o : $(COMMON_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $(OUTPUT) $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o

