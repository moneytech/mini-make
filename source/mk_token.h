#ifndef mk_token_h
#define mk_token_h

/** This enumerates all of the token types.
 * @ingroup mk_internal
 * */
enum mk_token_type {
  /** The token is a ':' symbol. */
  MK_TOKEN_COLON,
  /** The token is a comment. */
  MK_TOKEN_COMMENT,
  /** THe token is a '$' symbol. */
  MK_TOKEN_DOLLAR_SIGN,
  /** The token is an identifier. */
  MK_TOKEN_IDENTIFIER,
  /** THe token is a '{' symbol. */
  MK_TOKEN_LEFT_CURLY_BRACKET,
  /** The token is a newline sequence. */
  MK_TOKEN_NEWLINE,
  /** The token is a number sequence. */
  MK_TOKEN_NUMBER,
  /** The token is a '}' symbol. */
  MK_TOKEN_RIGHT_CURLY_BRACKET,
  /** The token is a series of spaces and tabs. */
  MK_TOKEN_SPACE,
  /** The token is a symbol not
   * described by other token types. */
  MK_TOKEN_OTHER
};

/** This structure contains data
 * related to a token.
 * @ingroup mk_internal
 * */
struct mk_token
{
  /** Indicates the type of this token. */
  enum mk_token_type type;
  /** The token character data. */
  const char* data;
  /** The number of characters in the token. */
  size_t size;
  /** The line that the token belongs to. */
  size_t line;
  /** The column that the token belongs to. */
  size_t column;
};

#endif /* mk_token_h */
