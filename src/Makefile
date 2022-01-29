DEBUGFLAGS = -g -O2
ifeq ($(OS), minix)
CC = cc
CFLAGS = -Wall
VERSION = 0732
DISTFILE = list$(VERSION).tgz
DISTBINFILE = listb$(VERSION).tgz
else
CC = gcc
CFLAGS = -Wall -W -pedantic
OLDCFLAGS = -Wall -W -fdefault-inline -pedantic -nostdinc -I/home/chroot/usr/include
VERSION = 0.7.32
DISTFILE = list-$(VERSION).tar.gz
DISTBINFILE = list-bin-$(VERSION).tar.gz
endif
#SRCDIR = list-$(VERSION)
SRCDIR = src
LDFLAGS = -s
OLDFLAGS = -s -nostdlibs -L/home/chroot/usr/lib -lc
LIBS = -lcurses
# STATICLIBS = -l:libcurses.a -l:libtinfo.a -l:libgpm.a -static-libgcc
CCMALLOC = ccmalloc
OPTFLAGS = -O3
BIN = /usr/local/bin
MAN = /usr/local/man

OBJECTS = list.o dequeue.o bugwrite.o screen.o file.o input.o
DOBJECTS = listd.o dequeued.o bugwrited.o screend.o filed.o inputd.o
OLDOBJECTS = list.o dequeue.o bugwrite.o screen.o file.o input.o

all: list

debug: list_d

.c.o:
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<

$(DOBJECTS): %d.o: %.c
	$(CC) $(DEBUGFLAGS) $(CFLAGS) -c $< -o $@

list: $(OBJECTS)
	$(CC) $(LDFLAGS) -o list $(OBJECTS) $(LIBS)

oldlist: $(OLDOBJECTS)
	$(CC) $(OLDFLAGS) -o list $(OBJECTS) $(LIBS)

list_d: $(DOBJECTS)
	$(CC) -o list_d $(DOBJECTS) $(LIBS) 

# Comment this out, as it seems to choke on gpm functions linked in from ncurses
# list_static: $(OBJECTS)
#	$(CC) -static -o list_static $(OBJECTS) $(STATICLIBS)

list_profile: $(OBJECTS)
	$(CC) $(LDFLAGS) -o list $(OBJECTS) $(LIBS)

list_ccmalloc: $(DOBJECTS)
	 $(CCMALLOC) $(CC) $(LDFLAGS) -o clist $(DOBJECTS) $(LIBS)

install: list
	install -m755 list $(BIN)/list
	install -m644 list.1 $(MAN)/man1/list.1

dist: list.c screen.c file.c input.c list.h
	cd .. && tar czvf $(DISTFILE) $(SRCDIR)/list.1 $(SRCDIR)/list.c $(SRCDIR)/list.h $(SRCDIR)/dequeue.c $(SRCDIR)/dequeue.h $(SRCDIR)/filebrowse.c $(SRCDIR)/bugwrite.c $(SRCDIR)/Makefile $(SRCDIR)/BSDMakefile $(SRCDIR)/screen.c $(SRCDIR)/file.c $(SRCDIR)/input.c 

dist-bin: list list_static list_d
	cd .. && tar czvf $(DISTBINFILE) $(SRCDIR)/list $(SRCDIR)/list_static $(SRCDIR)/list_d $(SRCDIR)/list.1

clean:
	rm -f *.o list list_d list_static

uninstall:
	rm $(BIN)/list
	rm $(MAN)/man1/list.1

