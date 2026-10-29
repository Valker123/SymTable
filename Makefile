# Macros
CC = gcc217
CFLAGS = -g

all: testsymtablelist testsymtablehash

clean:
	rm -f testsymtable.o symtablelist.o symtablehash.o
	rm -f testsymtablelist testsymtablehash

clobber: clean
	rm -f *~ \#*\#

testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CFLAGS) testsymtable.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtable.o symtablehash.o
	$(CC) $(CFLAGS) testsymtable.o symtablehash.o -o testsymtablehash

testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c

symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c