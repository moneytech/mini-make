INCLUDEPATH += "include"

gcc {
  QMAKE_CFLAGS += -Werror -Wfatal-errors
}

win32-msvc* {
  QMAKE_CFLAGS += /WX
}

SOURCES += "mini-make.c"

HEADERS += "include/mini-make/assignment-stmt.h"
HEADERS += "include/mini-make/command.h"
HEADERS += "include/mini-make/include-stmt.h"
HEADERS += "include/mini-make/options.h"

HEADERS += "include/mini-make/chdir.h"
SOURCES += "chdir.c"

HEADERS += "include/mini-make/error.h"
SOURCES += "error.c"

HEADERS += "include/mini-make/interpreter.h"
SOURCES += "interpreter.c"

HEADERS += "include/mini-make/ihooks.h"
SOURCES += "ihooks.c"

HEADERS += "include/mini-make/job.h"
SOURCES += "job.c"

HEADERS += "include/mini-make/job-manager.h"
SOURCES += "job-manager.c"

HEADERS += "include/mini-make/phooks.h"
SOURCES += "phooks.c"

HEADERS += "include/mini-make/parser.h"
SOURCES += "parser.c"

HEADERS += "include/mini-make/string.h"
SOURCES += "string.c"

HEADERS += "include/mini-make/table.h"
SOURCES += "table.c"

HEADERS += "include/mini-make/target.h"
SOURCES += "target.c"

HEADERS += "include/mini-make/var.h"
SOURCES += "var.c"

TARGET = "mini-make"
