CC = gcc
CFLAG = -g -Wall

TARGET = bin/my_server
SERVER_OBJECT = obj/server.o
SERVER_SOURCE = src/server.c include/server.h
MAIN_SOURCE = src/main.c

$(SERVER_OBJECT): $(SERVER_SOURCE)
	$(CC) $(CFLAG) -c $< -o $@

APP: $(MAIN_SOURCE) $(SERVER_OBJECT)
	$(CC) $(CFLAG) $^ -o $(TARGET)

RUN: 
	./$(TARGET)