CC = gcc
CFLAGS = -Wall -Wextra -O2
CFLAGS_DEBUG = -Wall -Werror -Wextra -g -fsanitize=address,undefined
COMPILE_OBJ = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@
COMPILE_EXC = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(INCDIR) -o $@ $^
COMPILE_LIB = @mkdir -p $(BLDDIR) && ar rcs $@ $^
BLDDIR = build
SRCDIR = src
INCDIR = include
TSTDIR = tests
PREFIX = /usr/local

$(BLDDIR)/libpurpcliopts.a: $(BLDDIR)/purpcliopts.o
	$(COMPILE_LIB)

$(BLDDIR)/test: $(TSTDIR)/test.c $(BLDDIR)/libpurpcliopts.a $(INCDIR)/purpcliopts.h
	@mkdir -p $(BLDDIR)
	$(CC) $(CFLAGS_DEBUG) -I$(INCDIR) $< -L$(BLDDIR) -lpurpcliopts -o $@

$(BLDDIR)/purpcliopts.o: $(SRCDIR)/purpcliopts.c $(INCDIR)/purpcliopts.h
	$(COMPILE_OBJ)

.PHONY: check debug install uninstall clean

check:
	LC_ALL=C ./checkpatch.pl --strict --ignore LONG_LINE_STRING --ignore SPDX_LICENSE_TAG --no-tree -f src/*.c include/*.h tests/*.c

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
