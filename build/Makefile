#
#! gochomugo
# I will make your Day
#

CFLAGS += -Ideps `pkg-config --cflags libgit2 libiniparser` -I../deps/
CFLAGS += --std=c99 -Wall -pedantic
LIBS    = `pkg-config --libs libgit2 libiniparser`
SRC     = ../src/*.c ../deps/**/*.c
PREFIX ?= /usr/local

bh: $(SRC)
	$(CC) -c $? $(CFLAGS)
	$(CC) -o $@ *.o $(CFLAGS) $(LIBS)

install: bh
	install bh $(PREFIX)/bin/bh

clean:
	rm -rf *.gch *.o
