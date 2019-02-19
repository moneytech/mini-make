#include "mk_scan.h"

#include "mk_malloc.h"
#include "mk_parse.h"
#include "mk_token.h"

static int mk_isalpha(char c) {
  return ((c >= 'a') && (c <= 'z'))
      || ((c >= 'A') && (c <= 'Z'));
}

static int mk_isdigit(char c) {
  return (c >= '0') && (c <= '9');
}

struct mk_scanner {
  struct mk_state* state;
  const char* data;
  size_t data_size;
  size_t line;
  size_t column;
};

int mk_scanner_done(const struct mk_scanner* scanner) {
  return !scanner->data_size;
}

static size_t mk_identifier_size(struct mk_scanner* scanner) {

  size_t size = 1;

  while (size < scanner->data_size) {
    char c = scanner->data[size];
    if (!mk_isdigit(c)
     && !mk_isalpha(c)
     && (c != '_')) {
      break;
    }
    size++;
  }

  return size;
}

static size_t mk_comment_size(struct mk_scanner* scanner) {

  size_t size = 1;

  while (size < scanner->data_size) {
    char c = scanner->data[size];
    if ((c == '\r') || (c == '\n')) {
      break;
    }
    size++;
  }

  return size;
}

static int mk_scan_token(struct mk_scanner* scanner, struct mk_token* token) {

  token->data = scanner->data;
  token->line = scanner->line;
  token->column = scanner->column;

  char first_char = token->data[0];

  switch (first_char) {
    case ':':
      token->type = MK_TOKEN_COLON;
      token->size = 1;
      break;
    case '{':
      token->type = MK_TOKEN_LEFT_CURLY_BRACKET;
      token->size = 1;
      break;
    case '}':
      token->type = MK_TOKEN_RIGHT_CURLY_BRACKET;
      token->size = 1;
      break;
    case '$':
      token->type = MK_TOKEN_DOLLAR_SIGN;
      token->size = 1;
      break;
    case '\n':
      token->type = MK_TOKEN_NEWLINE;
      token->size = 1;
      break;
    case '#':
      token->type = MK_TOKEN_COMMENT;
      token->size = mk_comment_size(scanner);
      break;
    default:
      if (mk_isalpha(first_char) || (first_char == '_')) {
        token->type = MK_TOKEN_IDENTIFIER;
        token->size = mk_identifier_size(scanner);
      } else if (mk_isdigit(first_char)) {
        token->type = MK_TOKEN_NUMBER;
        token->size = mk_identifier_size(scanner);
      } else {
        token->type = MK_TOKEN_OTHER;
        token->size = 1;
      }
      break;
  }

  for (size_t i = 0; i < token->size; i++) {
    char c = token->data[i];
    if (c == '\n') {
      scanner->line++;
      scanner->column = 1;
    } else if (c > 0) {
      scanner->column++;
    }
  }

  scanner->data++;
  scanner->data_size -= token->size;

  return 0;
}

int mk_scan(struct mk_state* state, const char* data, size_t size) {

  struct mk_scanner scanner;
  scanner.state = state;
  scanner.data = data;
  scanner.data_size = size;
  scanner.line = 1;
  scanner.column = 1;

  struct mk_token* token_array = NULL;
  size_t token_count = 0;
  size_t token_max = 0;

  struct mk_token token;

  int err = 0;

  while (!mk_scanner_done(&scanner)) {

    err = mk_scan_token(&scanner, &token);
    if (err) {
      break;
    }

    if (token_count >= token_max) {

      token_max += 16;

      struct mk_token* tmp = mk_realloc(state, token_array, token_max * sizeof(struct mk_token));
      if (!tmp) {
        mk_free(state, token_array);
        return -1;
      }

      token_array = tmp;
    }

    token_array[token_count++] = token;
  }

  if (err) {
    mk_free(state, token_array);
    return err;
  }

  err = mk_parse(state, token_array, token_count);

  mk_free(state, token_array);

  return err;
}
