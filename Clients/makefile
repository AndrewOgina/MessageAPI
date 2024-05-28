CC = gcc
CFLAG = -g -Wall
TARGET = bin/client
CLIENT_OBJECT = obj/client.o
CLIENT_SOURCE = src/client.c include/client.h
MAIN_SOURCE = src/main.c 
LOCALHOST = 127.0.0.1

$(CLIENT_OBJECT): $(CLIENT_SOURCE)
	$(CC) $(CFLAG) -c $< -o $@ 

APP: $(MAIN_SOURCE) $(CLIENT_OBJECT)
	$(CC) $(CFLAG) $^ -o $(TARGET)
