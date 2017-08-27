INCLUDEPATH += "include"

SOURCES += "make.c"

HEADERS += "include/make/assignment-stmt.h"
HEADERS += "include/make/command.h"
HEADERS += "include/make/include-stmt.h"
HEADERS += "include/make/options.h"

HEADERS += "include/make/chdir.h"
SOURCES += "chdir.c"

HEADERS += "include/make/interpreter.h"
SOURCES += "interpreter.c"

HEADERS += "include/make/job.h"
SOURCES += "job.c"

HEADERS += "include/make/job-manager.h"
SOURCES += "job-manager.c"

HEADERS += "include/make/listener.h"
SOURCES += "listener.c"

HEADERS += "include/make/parser.h"
SOURCES += "parser.c"

HEADERS += "include/make/string.h"
SOURCES += "string.c"

HEADERS += "include/make/table.h"
SOURCES += "table.c"

HEADERS += "include/make/var.h"
SOURCES += "var.c"

TARGET = "mini-make"
