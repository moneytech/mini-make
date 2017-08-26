#ifndef MAKE_PARSER_H
#define MAKE_PARSER_H

struct make_string;
struct make_listener;

struct make_parser {
  struct make_string *source;
  struct make_listener *listener;
};

void make_parser_init(struct make_parser *parser);

int make_parser_run(struct make_parser *parser);

#endif /* MAKE_PARSER_H */
