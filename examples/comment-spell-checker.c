/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <mini-make/error.h>
#include <mini-make/parser.h>

#include <aspell.h>

struct spell_checker {
  AspellConfig *config;
  AspellSpeller *speller;
};

static int spell_checker_init(struct spell_checker *checker) {

  AspellCanHaveError *ret;

  checker->config = NULL;
  checker->speller = NULL;

  checker->config = new_aspell_config();
  if (checker->config == NULL) {
    fprintf(stderr, "Failed to create Aspell config\n");
    return -EFAULT;
  }

  ret = new_aspell_speller(checker->config);

  delete_aspell_config(checker->config);

  if (aspell_error(ret) != 0) {
    fprintf(stderr, "Aspell error: %s\n", aspell_error_message(ret));
    delete_aspell_can_have_error(ret);
    return EXIT_FAILURE;
  }

  checker->speller = to_aspell_speller(ret);

  checker->config = aspell_speller_config(checker->speller);

  return EXIT_SUCCESS;
}

static void spell_checker_free(struct spell_checker *checker) {
  if (checker->speller != NULL) {
    delete_aspell_speller(checker->speller);
    checker->speller = NULL;
  }
}

static int on_comment(void *data, const struct make_string *comment) {

  struct spell_checker *checker;
  const char *str;
  size_t str_size;
  size_t i;
  const char *word;
  size_t word_size;
  int good_word;

  checker = (struct spell_checker *) data;

  str = comment->data;
  str_size = comment->size;
  for (i = 0; i < str_size; i++) {
    /* Skip to the first letter */
    while (i < str_size) {
      if (isalpha(str[i]))
        break;
      else
        i++;
    }
    word = &str[i];
    word_size = 0;
    while ((i < str_size) && isalpha(str[i])) {
      word_size++;
      i++;
    }

    if (word_size > 1) {
      good_word = aspell_speller_check(checker->speller, word, word_size);
      if (good_word == 1) {
        /* nothing to do here */
      } else if (good_word == 0) {
        printf("Found malformed word: %.*s\n", (int) word_size, word);
      } else {
        /* error */
      }
    }
  }

  return make_success;
}

static int spell_check(const char *path) {

  int err;
  struct make_parser parser;
  struct spell_checker checker;

  err = spell_checker_init(&checker);
  if (err)
    return EXIT_FAILURE;

  make_parser_init(&parser);

  parser.hooks.data = &checker;
  parser.hooks.on_comment = on_comment;

  err = make_parser_read(&parser, path);
  if (err) {
    fprintf(stderr, "Failed to read '%s'\n", path);
    make_parser_free(&parser);
    spell_checker_free(&checker);
    return EXIT_FAILURE;
  }

  err = make_parser_run(&parser);
  if (err) {
    fprintf(stderr, "Failed to parse '%s'\n", path);
    make_parser_free(&parser);
    spell_checker_free(&checker);
    return EXIT_FAILURE;
  }

  make_parser_free(&parser);
  spell_checker_free(&checker);

  return EXIT_SUCCESS;
}

int main(int argc, const char **argv) {

  int i;
  int err;

  for (i = 1; i < argc; i++) {
    err = spell_check(argv[i]);
    if (err) {
      fprintf(stderr, "Failed to spell check '%s'\n", argv[i]);
      return err;
    }
  }

  return 0;
}

