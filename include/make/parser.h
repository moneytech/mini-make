#ifndef MAKE_PARSER_H
#define MAKE_PARSER_H

#include <make/listener.h>
#include <make/string.h>

struct make_parser {
  struct make_string source;
  struct make_listener listener;
};

void make_parser_init(struct make_parser *parser);

void make_parser_free(struct make_parser *parser);

int make_parser_read(struct make_parser *parser,
                     const char *filename);

int make_parser_run(struct make_parser *parser);

#endif /* MAKE_PARSER_H */
