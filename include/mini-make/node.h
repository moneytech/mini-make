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

/** @file */

#ifndef MINI_MAKE_NODE_H
#define MINI_MAKE_NODE_H

#include <mini-make/assignment-stmt.h>
#include <mini-make/include-stmt.h>
#include <mini-make/rule.h>
#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-node Make Node
 * @brief An element within a makefile.
 * */

/** @brief Describes the type of a
 * node.
 * @ingroup make-node
 * */
enum make_node_type {
  /** @brief The node is an assignment statement. */
  MAKE_ASSIGNMENT_STMT,
  /** @brief The node is a comment. */
  MAKE_COMMENT,
  /** @brief The node is an include statement. */
  MAKE_INCLUDE_STMT,
  /** @brief The node is a rule. */
  MAKE_RULE
};

/** @brief A makefile node. A makefile
 * node can be either a rule, assignment
 * statement or an include statement.
 * @ingroup make-node
 * */
struct make_node {
  /** @brief Indicates the type of the node. */
  enum make_node_type type;
  /** @brief Valid if the type is @ref MAKE_ASSIGNMENT_STMT */
  struct make_assignment_stmt assignment_stmt;
  /** @brief Valid if the type is @ref MAKE_COMMENT */
  struct make_string comment;
  /** @brief Valid if the type is @ref MAKE_INCLUDE_STMT */
  struct make_include_stmt include_stmt;
  /** @brief Valid if the type is @ref MAKE_RULE */
  struct make_rule rule;
};

/** @brief Initializes a makefile node.
 * When the node is initialized, it is
 * initialized as a rule.
 * @ingroup make-node
 * */
void make_node_init(struct make_node *node);

/** @brief Releases all resources allocated by
 * the node.
 * @ingroup make-node
 * */
void make_node_free(struct make_node *node);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_NODE_H */
