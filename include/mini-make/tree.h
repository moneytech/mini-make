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

#ifndef MINI_MAKE_TREE_H
#define MINI_MAKE_TREE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_node;

/** @brief A makefile abstract syntax tree. */
struct make_tree {
  /** @brief Nodes within the tree. */
  struct make_node *node_array;
  /** @brief The number of nodes in the tree. */
  size_t node_count;
};

void make_tree_init(struct make_tree *tree);

void make_tree_free(struct make_tree *tree);

struct make_node *make_tree_get_node(struct make_tree *tree, size_t index);

size_t make_tree_get_node_count(const struct make_tree *tree);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_TREE_H */
