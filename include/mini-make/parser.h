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

/** @file parser.h
 * @brief Contains functions and
 * declarations related to the parser.
 * */

/** @defgroup make-parser Make Parser
 * @brief Contains functions and
 * declarations related to the parser.
 * */

#ifndef MINI_MAKE_PARSER_H
#define MINI_MAKE_PARSER_H

#include <mini-make/phooks.h>
#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_tree;

/** @brief Used for parsing makefiles. This
 * structure is used by the interpreter to run
 * a makefile.To use the parser directly, initialize
 * a @ref make-phooks structure with the appropriate
 * callbacks and then call @ref make_parser_set_hooks.
 * @ingroup make-parser
 * */
struct make_parser {
  /** @brief The file name (or path) of the
   * makefile that is being parsed. */
  struct make_string path;
  /** @brief The makefile source code. */
  struct make_string source;
  /** @brief Contains callback functions
   * that the parser will call as it parses
   * the makefile source. */
  struct make_phooks hooks;
};

/** @brief Initializes the contents of the parser.
 * @ingroup make-parser
 * */
void make_parser_init(struct make_parser *parser);

/** @brief Releases all resources allocated by the
 * parser. */
void make_parser_free(struct make_parser *parser);

/** @brief Constructs a makefile AST from the
 * makefile source.
 * @ingroup make-parser
 * */
int make_parser_build_tree(struct make_parser *parser,
                           struct make_tree *tree);

/** @brief Reads the contents of @p filename and
 * sets the path of the file.
 * @ingroup make-parser
 * */
int make_parser_read(struct make_parser *parser,
                     const char *filename);

/** @brief Parses the makefile source.
 * @ingroup make-parser
 * */
int make_parser_run(struct make_parser *parser);

/** @brief Sets the parser hooks. This function
 * will not set a hook if it is set to NULL.
 * @ingroup make-parser
 * */
void make_parser_set_hooks(struct make_parser *parser,
                           const struct make_phooks *hooks);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_PARSER_H */
