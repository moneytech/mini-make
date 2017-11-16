DESTDIR ?=
PREFIX ?= /usr/local
VPATH += include/mini-make

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
	./parser-test --makefile testing/makefiles/test1/Makefile
	./table-test

.PHONY: install
install: mini-make
	install mini-make $(DESTDIR)$(PREFIX)/bin/mini-make

mini-make: mini-make.o libmake.a

libmake.a: chdir.o \
           error.o \
           interpreter.o \
           ihooks.o \
           job.o \
           job-manager.o \
           phooks.o \
           mini-make.o \
           parser.o \
           string.o \
           table.o \
           target.o \
           var.o
	$(AR) $(ARFLAGS) $@ $^

chdir.o: chdir.c chdir.h

error.o: error.c error.h

interpreter.o: interpreter.c interpreter.h

ihooks.o: ihooks.c ihooks.h

interpreter-test.o: interpreter-test.c interpreter.h

job.o: job.c job.h

job-manager.o: job-manager.c job-manager.h

phooks.o: phooks.c phooks.h

mini-make.o: mini-make.c

parser.o: parser.c parser.h

parser-test.o: parser-test.c

string.o: string.c string.h

table.o: table.c table.h

target.o: target.c target.h

table-test.o: table-test.c

var.o: var.c var.h

interpreter-test: interpreter-test.o -lmake

parser-test: parser-test.o -lmake

table-test: table-test.o -lmake

