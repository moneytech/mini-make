CC = gcc
CFLAGS += -Wall -Wextra -Werror -Wfatal-errors
CFLAGS += -ansi -pedantic
CFLAGS += -fPIC
CFLAGS += -Wl,-rpath=.
CFLAGS += -g
CFLAGS += -Iinclude

VPATH += include/make

.PHONY: all
all: libmake.so parser-test

libmake.so: parser.o
	$(CC) -shared $^ -o $@

parser.o: parser.c parser.h string.h listener.h

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.so
	$(RM) *-test

.PHONY: test
test: parser-test
	./parser-test

parser-test: parser-test.c -lmake

