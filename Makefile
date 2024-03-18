CC = gcc
CFLAGS = -g

all: client server

client: client.c 
	$(CC) $(CFLAGS) -o client.o client.c

server: server.c
	$(CC) $(CFLAGS) -o server.o server.c