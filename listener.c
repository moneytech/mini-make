#include <make/listener.h>

#include <stdio.h>

static void on_unexpected_char(void *data, char c) {
  (void) data;
  fprintf(stderr, "Unexpected character '%c'\n", c);
}

void make_listener_init(struct make_listener *listener) {
  listener->on_unexpected_char = on_unexpected_char;
}

