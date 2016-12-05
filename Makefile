PROGS = smoketest vit-spinka-client1 echo-client ondra

CFLAGS = -I/usr/local/include -std=c99
CXXFLAGS = -I/usr/local/include -std=c99
LDFLAGS = -L/usr/local/lib -lczmq -lzmq -lmlm

all: $(PROGS)
