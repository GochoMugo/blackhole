#
#! gochomugo
# I will make your Day
#

ROOT         = $(PWD)/..
PREFIX      ?= /usr/local

CFLAGS      += `pkg-config --cflags libgit2 libiniparser` -I$(ROOT)/deps/
CFLAGS      += --std=c99 -Wall -pedantic
LIBS         = `pkg-config --libs libgit2 libiniparser`
SRC          = $(ROOT)/src/*.c $(ROOT)/deps/**/*.c
OUT          = $(PWD)/src
EXE          = $(PWD)/bh
LIB          = $(OUT)/libblackhole.a

CFLAGS_TEST  = `pkg-config --cflags cmocka`
LIBS_TEST    = `pkg-config --libs cmocka` -L$(OUT) -lblackhole $(LIBS)
SRC_TEST     = $(ROOT)/test/*.c
OUT_TEST     = $(PWD)/test
EXE_TEST     = $(PWD)/bh-tests


.PHONY: clean install pc veryclean


$(EXE): $(SRC)
	mkdir -p $(OUT)
	cd $(OUT) && $(CC) -c $? $(CFLAGS)
	cd $(OUT) && $(AR) cr $(LIB) *.o
	cd $(OUT) && $(CC) -o $@ *.o $(CFLAGS) $(LIBS)

install: $(EXE)
	install $(EXE) $(PREFIX)/bin/bh

pc:
	mv $(ROOT)/misc/*.pc $(PREFIX)/lib/pkgconfig/

test: $(EXE) $(EXE_TEST)
	$(EXE_TEST)

$(EXE_TEST): $(SRC_TEST)
	mkdir -p $(OUT_TEST)
	cd $(OUT_TEST) && $(CC) -c $? $(CFLAGS_TEST)
	cd $(OUT_TEST) && $(CC) -o $@ *.o $(CFLAGS_TEST) $(LIBS_TEST)

clean:
	$(RM) -r $(OUT) $(OUT_TEST)

veryclean: clean
	$(RM) $(EXE) $(EXE_TEST)
