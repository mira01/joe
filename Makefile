PROGS = smoketest vit-spinka-client1 echoclient ondra

CC = gcc
CXX= g++
prefix ?= /usr/local
CFLAGS = -I$(prefix)/include -std=c99
CXXFLAGS = -I$(prefix)/include -std=c99
LDFLAGS = -L$(prefix)/lib -R$(prefix)/lib -lczmq -lzmq -lmlm

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
