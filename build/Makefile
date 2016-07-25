#
#! gochomugo
# I will make your Day
#

ROOT         = $(PWD)/..
PREFIX      ?= /usr/local
SCRIPTS      = $(ROOT)/script
GEN_HEADERS  = $(SCRIPTS)/gen-test-headers.sh
ECHO         = $(SCRIPTS)/echo-task.sh

CFLAGS      += `pkg-config --cflags libgit2 libiniparser` -I$(ROOT)/deps/
CFLAGS      += --std=c99 -Wall -pedantic
CFLAGS      += -D"LIBINIPARSER_VERSION=\"`pkg-config --modversion libiniparser || echo ?`\""
LIBS         = `pkg-config --libs libgit2 libiniparser`
SRC          = $(ROOT)/src/*.c $(ROOT)/deps/**/*.c
OUT          = $(PWD)/src
EXE          = $(PWD)/bh
LIB          = $(OUT)/libblackhole.a

CFLAGS_TEST  = `pkg-config --cflags cmocka`
LIBS_TEST    = `pkg-config --libs cmocka` -L$(OUT) -lblackhole $(LIBS)
TEST_DIR     = $(ROOT)/test
SRC_TEST     = $(TEST_DIR)/*.c
OUT_TEST     = $(PWD)/test
EXE_TEST     = $(PWD)/bh-tests


.PHONY: clean install pc veryclean


$(EXE): $(SRC)
	@mkdir -p $(OUT)
	@$(ECHO) CC $? ; cd $(OUT) && $(CC) -c $? $(CFLAGS)
	@$(ECHO) AR $(LIB) ; cd $(OUT) && $(AR) cr $(LIB) *.o
	@$(ECHO) CC $@ ; cd $(OUT) && $(CC) -o $@ *.o $(CFLAGS) $(LIBS)

install: $(EXE)
	install $(EXE) $(PREFIX)/bin/bh

pc:
	mv $(ROOT)/misc/*.pc $(PREFIX)/lib/pkgconfig/

test: $(EXE) $(EXE_TEST)
	@$(ECHO) SCRIPT $(TEST_DIR)/setup.sh ; cd $(TEST_DIR) && CC="$(CC)" CFLAGS="$(CFLAGS_TEST)" LIBS="$(LIBS_TEST)" ./setup.sh
	@$(ECHO) RUN $(EXE_TEST) ; TEST_DIR=$(TEST_DIR) \
		$(EXE_TEST)

$(EXE_TEST): $(SRC_TEST)
	@mkdir -p $(OUT_TEST)
	@$(ECHO) SCRIPT $(GEN_HEADERS) ; $(GEN_HEADERS) $(SRC_TEST)
	@$(ECHO) CC $? ; cd $(OUT_TEST) && $(CC) -c $? $(CFLAGS_TEST) -g
	@$(ECHO) CC $@ ; cd $(OUT_TEST) && $(CC) -o $@ *.o $(CFLAGS_TEST) $(LIBS_TEST)

clean:
	@$(RM) -r $(OUT) $(OUT_TEST) \
		$(TEST_DIR)/data/**.tmp.counter $(TEST_DIR)/data/**/.git/ \
		$(TEST_DIR)/data/**/*.out $(TEST_DIR)/data/*.out \
		`$(SCRIPTS)/which-test-headers.sh $(SRC_TEST)` \
		$(TEST_DIR)/data/bh-git/.gitignore

veryclean: clean
	@$(RM) $(EXE) $(EXE_TEST)
