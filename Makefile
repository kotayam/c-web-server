CC = gcc
CFLAGS = -g

all: client server test

client: client.c 
	$(CC) $(CFLAGS) -o client.o client.c

server: server.c helper.c
	$(CC) $(CFLAGS) -o main.o server.c helper.c

test: test.c helper.c 
	$(CC) $(CFLAGS) -o test.o test.c helper.c