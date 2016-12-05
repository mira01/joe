PROGS = smoketest vit-spinka-client1
CFLAGS = -I/usr/local/include -std=c99
CXXFLAGS = -I/usr/local/include -std=c99
LDFLAGS = -L/usr/local/lib -lczmq -lzmq -lmlm

all: $(PROGS)
