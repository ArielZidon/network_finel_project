.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: send recv client server lookup web source gateway sink

send: send_udp.o
	$(CC) $< -o send 

recv: recv_udp.o
	$(CC) $< -o recv

client: net_client.o
	$(CC) $< -o client

server: net_server.o
	$(CC) $< -o server

lookup: nslookup.o
	$(CC) $< -o lookup

web: web.c
	$(CC) $< -o web

source: source.c
	$(CC) $< -o source

gateway: gateway.c
	$(CC) $< -o gateway

sink: sink.c
	$(CC) $< -o sink
%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o send recv client server lookup web source gateway sink