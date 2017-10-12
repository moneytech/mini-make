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

#include <mini-make/tree.h>

#include <mini-make/node.h>

void make_tree_init(struct make_tree *tree) {
  tree->node_array = NULL;
  tree->node_count = 0;
}

void make_tree_done(struct make_tree *tree) {

  for (size_t i = 0; i < tree->node_count; i++)
    make_node_free(&tree->node_array[i]);

  free(tree->node_array);
  tree->node_array = NULL;
  tree->node_count = 0;
}

struct make_node *make_tree_get_node(struct make_tree *tree, size_t index) {
  if (index >= tree->node_count)
    return NULL;
  else
    return &tree->node_array[index];
}

size_t make_tree_get_node_count(const struct make_tree *tree) {
  return tree->node_count;
}

