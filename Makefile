PROGS = smoketest vit-spinka-client1 echoclient ondra echobara echosrv fmq

CC = gcc
CXX= g++
prefix ?= /usr/local
CFLAGS = -I$(prefix)/include -std=c99
CXXFLAGS = -I$(prefix)/include -std=c99
LDFLAGS = -L$(prefix)/lib -lczmq -lzmq -lmlm

# Travis gcc does not like -R
#LDFLAGS += -R$(prefix)/lib

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
