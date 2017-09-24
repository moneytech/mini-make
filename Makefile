DESTDIR ?=
PREFIX ?= /usr/local

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors
CFLAGS += -Iinclude

ifndef MINI_MAKE_RELEASE
CFLAGS += -g
endif

tests += interpreter-test
tests += parser-test
tests += table-test

.PHONY: all
all: mini-make
	$(MAKE) -C examples all

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) libmake.a
	$(RM) mini-make
	$(RM) $(tests)
	$(MAKE) -C examples clean

.PHONY: test
test: $(tests)
	./interpreter-test
	./parser-test --makefile test.mk
	./table-test

.PHONY: install
install: mini-make
	install mini-make $(DESTDIR)$(PREFIX)/bin/mini-make

mini-make: mini-make.o libmake.a

libmake.a: chdir.o \
           error.o \
           interpreter.o \
           interpreter-test.o \
           job.o \
           job-manager.o \
           phooks.o \
           mini-make.o \
           parser.o \
           parser-test.o \
           string.o \
           table.o \
           table-test.o \
           var.o
	$(AR) $(ARFLAGS) $@ $^

chdir.o: chdir.c

error.o: error.c

interpreter.o: interpreter.c

interpreter-test.o: interpreter-test.c

job.o: job.c

job-manager.o: job-manager.c

phooks.o: phooks.c

mini-make.o: mini-make.c

parser.o: parser.c

parser-test.o: parser-test.c

string.o: string.c

table.o: table.c

table-test.o: table-test.c

var.o: var.c

interpreter-test: interpreter-test.o -lmake

parser-test: parser-test.o -lmake

table-test: table-test.o -lmake

