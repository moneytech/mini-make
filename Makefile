CC = gcc
CFLAGS += -Wall -Wextra -Werror -Wfatal-errors
CFLAGS += -ansi -pedantic
CFLAGS += -fPIC
CFLAGS += -Wl,-rpath=.
CFLAGS += -g
CFLAGS += -Iinclude

VPATH += include/make

.PHONY: all
all: libmake.so make interpreter-test parser-test table-test

libmake.so: interpreter.o parser.o string.o table.o
	$(CC) -shared $^ -o $@

interpreter.o: interpreter.c interpreter.h

make: make.c -lmake
	$(CC) $(CFLAGS) $^ -o $@

make.o: parser.h string.h listener.h

parser.o: parser.c parser.h string.h listener.h

string.o: string.c string.h

table.o: table.c table.h

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.so
	$(RM) *-test

.PHONY: test
test: parser-test interpreter-test table-test
	./parser-test
	./table-test
	./interpreter-test

parser-test: parser-test.c -lmake

interpreter-test: interpreter-test.c -lmake

table-test: table-test.c -lmake

