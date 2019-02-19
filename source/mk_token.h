#ifndef mk_token_h
#define mk_token_h

enum mk_token_type {
  MK_TOKEN_COLON,
  MK_TOKEN_COMMENT,
  MK_TOKEN_DOLLAR_SIGN,
  MK_TOKEN_IDENTIFIER,
  MK_TOKEN_LEFT_CURLY_BRACKET,
  MK_TOKEN_NEWLINE,
  MK_TOKEN_NUMBER,
  MK_TOKEN_RIGHT_CURLY_BRACKET,
  MK_TOKEN_SPACE,
  MK_TOKEN_OTHER
};

struct mk_token
{
  enum mk_token_type type;
  const char* data;
  size_t size;
  size_t line;
  size_t column;
};

#endif /* mk_token_h */
