#include "egl_bstree.h"

#include <errno.h>
#include <stdlib.h>

/*
 * Free the egl_bstree and values it contains
 */

static void egl_bstree_free(struct egl_bstree *tree) {
  if (tree == NULL)
    return;

  egl_bstree_free(tree->left);
  egl_bstree_free(tree->right);

  if (tree->value != NULL)
    free(tree->value);
  free(tree);
}

/*
 * Add a given value to a given egl_bstree
 * Returns NULL if a problem occurred
 */
static struct egl_bstree *egl_bstree_add(struct egl_bstree *tree, void *i,
                                         int (*compare)(const void *a,
                                                        const void *b)) {
  if (tree == NULL)
    return egl_bstree_new(i);
  if (compare(i, tree->value) < 0) {
    tree->left = egl_bstree_add(tree->left, i, compare);
  } else {
    tree->right = egl_bstree_add(tree->right, i, compare);
  }

  return tree;
}

/*
 * Find a given value in an egl_bstree using a specified comparison function
 * Returns NULL if the value is not found
 */

static egl_bstree *egl_bstree_find(struct egl_bstree *const tree, const void *i,
                                   int (*compare)(const void *, const void *)) {
  if (tree == NULL)
    return NULL;

  if (tree->value == NULL)
    return NULL;

  if (i == NULL)
    return NULL;

  if (compare(i, tree->value) == 0) {
    return tree;
  }

  if (compare(i, tree->value) < 0) {
    return egl_bstree_find(tree->left, i, compare);
  } else {
    return egl_bstree_find(tree->right, i, compare);
  }
}

/*
 * Delete a value from a given egl_bstree, if it exists
 * On success, returns the root of the tree
 * Returns NULL if a problem occurred
 */

static egl_bstree *egl_bstree_remove(struct egl_bstree *tree, void *val,
                                     int (*compare)(const void *a,
                                                    const void *b)) {
  if (tree == NULL) {
    return NULL;
  }

  if (compare(val, tree->value) > 0) {
    tree->right = tree->right->remove(tree->right, val, compare);
  } else if (compare(val, tree->value) < 0) {
    tree->left = tree->left->remove(tree->left, val, compare);
  } else {
    if (tree->left == NULL && tree->right == NULL) {
      free(tree->value);
      free(tree);
      return NULL;
    } else if (tree->left == NULL || tree->right == NULL) {
      egl_bstree *tmp;
      if (tree->left == NULL) {
        tmp = tree->right;
      } else {
        tmp = tree->left;
      }
      free(tree->value);
      free(tree);
      return tmp;
    } else {
      egl_bstree *tmp = tree->right->minimum(tree->right);
      tree->value = tmp->value;
      tree->right = tree->right->remove(tree->right, tmp->value, compare);
    }
  }

  return tree;
}

/*
 * Get the minimum value from a given egl_bstree
 */

static egl_bstree *egl_bstree_minimum(struct egl_bstree *tree) {
  if (tree == NULL) {
    return NULL;
  } else if (tree->left != NULL) {
    return tree->left->minimum(tree->left);
  }
  return tree;
}

/*
 * Initialise a new egl_bstree
 * Set the root value to the specified value
 */

static egl_bstree *egl_bstree_init(egl_bstree *tree, void *val) {
  if (tree == NULL) {
    return NULL;
  }
  tree->value = val;
  tree->left = NULL;
  tree->right = NULL;
  tree->add = &egl_bstree_add;
  tree->free = &egl_bstree_free;
  tree->find = &egl_bstree_find;
  tree->remove = &egl_bstree_remove;
  tree->minimum = &egl_bstree_minimum;
  return tree;
}

/*
 * Create a new egl_bstree with a given value for the root
 * Returns NULL if a problem occurred
 */

egl_bstree *egl_bstree_new(void *val) {
  egl_bstree *tree = malloc(sizeof(*tree));
  return egl_bstree_init(tree, val);
}
