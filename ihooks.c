#include <mini-make/ihooks.h>

#include <stdlib.h>

void make_ihooks_init(struct make_ihooks *ihooks) {
  ihooks->data = NULL;
}

