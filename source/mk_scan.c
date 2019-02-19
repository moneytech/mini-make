#include "mk_scan.h"

#include "mk_malloc.h"
#include "mk_parse.h"
#include "mk_token.h"

struct mk_scanner;

struct mk_scanner {
  struct mk_state* state;
  const char* data;
  size_t data_size;
};

int mk_scanner_done(const struct mk_scanner* scanner) {
  return !scanner->data_size;
}

static int mk_scan_token(struct mk_scanner* scanner, struct mk_token* token) {

  char first_char = scanner->data[0];

  switch (first_char) {
    case ':':
      token->type = MK_TOKEN_COLON;
      break;
    case '{':
      token->type = MK_TOKEN_LEFT_CURLY_BRACKET;
      break;
    case '}':
      token->type = MK_TOKEN_RIGHT_CURLY_BRACKET;
      break;
    case '\n':
      token->type = MK_TOKEN_NEWLINE;
      break;
    case '#':
      token->type = MK_TOKEN_COMMENT;
      break;
    default:
      token->type = MK_TOKEN_OTHER;
      break;
  }

  token->data = scanner->data;
  token->size = 1;

  scanner->data++;
  scanner->data_size--;

  return 0;
}

int mk_scan(struct mk_state* state, const char* data, size_t size) {

  struct mk_scanner scanner;
  scanner.state = state;
  scanner.data = data;
  scanner.data_size = size;

  struct mk_token* token_array = NULL;
  size_t token_count = 0;

  struct mk_token token;

  int err = 0;

  while (!mk_scanner_done(&scanner)) {
    err = mk_scan_token(&scanner, &token);
    if (err) {
      break;
    }
  }

  if (err) {
    mk_free(state, token_array);
    return err;
  }

  err = mk_parse(state, token_array, token_count);

  mk_free(state, token_array);

  return err;
}
