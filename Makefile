CC = gcc
CFLAGS = -Wall -Wextra -O2
CFLAGS_DEBUG = -Wall -Werror -Wextra -g -fsanitize=address,undefined
COMPILE_OBJ = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -c $< -o $@
COMPILE_EXC = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -o $@ $^
COMPILE_LIB = @mkdir -p $(BLDDIR) && ar rcs $@ $^
BLDDIR = build
SRCDIR = src
INCDIR = include
TSTDIR = tests
PREFIX = /usr/local

$(BLDDIR)/libpurpcliopts.a: $(BLDDIR)/purpcliopts.o
	$(COMPILE_LIB)

$(BLDDIR)/test: $(TSTDIR)/test.c $(BLDDIR)/libpurpcliopts.a
	@mkdir -p $(BLDDIR)
	$(CC) $(CFLAGS_DEBUG) -I$(INCDIR) $< -L$(BLDDIR) -lpurpcliopts -o $@

$(BLDDIR)/purpcliopts.o: $(SRCDIR)/purpcliopts.c $(INCDIR)/purpcliopts.h
	$(COMPILE_OBJ)

.PHONY: debug install uninstall clean

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(BLDDIR)/libpurpcliopts.a $(BLDDIR)/test

install:
	cp libpurpcliopts.a $(PREFIX)/lib/
	ranlib $(PREFIX)/lib/libpurpcliopts.a
	cp $(INCDIR)/this_file_doesnt_exist.h $(PREFIX)/include

uninstall:
	rm -f $(PREFIX)/lib/libpurpcliopts.a
	rm -f $(PREFIX)/include/this_file_doesnt_exist.h

clean:
	rm -rf $(BLDDIR)
