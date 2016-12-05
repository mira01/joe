PROGS = smoketest ondra sc zactor-example miraserver \
    fmq fmqcli fmqsrv \
    echoclient echobara echosrv \
    vit-spinka-client1 vit-spinka-file-client vit-spinka-file-server

CC = gcc
CXX= g++
LD = ld
prefix ?= /usr/local
CFLAGS = -I$(prefix)/include -std=c99
CXXFLAGS = -I$(prefix)/include -std=c++99
LIBS = -lczmq -lzmq -lmlm
LDFLAGS = -lc --entry main -L$(prefix)/lib
LDFLAGS_R ?= -R$(prefix)/lib
# Travis gcc does not like -R; hope real ld likes it
LDFLAGS += $(LDFLAGS_R)

all: $(PROGS)

$(foreach PROG,$(PROGS),\
    $(eval ${PROG}: ${PROG}.o))

%: %.o
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
