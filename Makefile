PROGS = smoketest vit-spinka-client1 echoclient ondra echobara echosrv fmq

CC = gcc
CXX= g++
CFLAGS = -I/usr/local/include -std=c99
CXXFLAGS = -I/usr/local/include -std=c99
LDFLAGS = -L/usr/local/lib -R/usr/local/lib -lczmq -lzmq -lmlm

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
