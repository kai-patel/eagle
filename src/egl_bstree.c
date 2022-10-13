#include "egl_bstree.h"

#include <errno.h>
#include <stdlib.h>

/*
 * Get the height of a given node in the egl_bstree
 * If the node does not exists, default to 0
 */

static int height(struct egl_bstree *tree) {
  if (tree == NULL)
    return 0;

  return tree->height;
}

/*
 * Find the maximum of two integers
 */

static int maximum(const int a, const int b) { return (a > b) ? a : b; }

/*
 * Right-rotate an AVL-node
 */

static egl_bstree *right_rotate(egl_bstree *y) {
  egl_bstree *x = y->left;
  egl_bstree *T2 = x->right;

  // Perform rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = maximum(height(y->left), height(y->right)) + 1;
  x->height = maximum(height(x->left), height(x->right)) + 1;

  // Return new root
  return x;
}

/*
 * Left-rotate an AVL-node
 */

static egl_bstree *left_rotate(egl_bstree *x) {
  egl_bstree *y = x->right;
  egl_bstree *T2 = y->left;

  // Perform rotation
  y->left = x;
  x->right = T2;

  //  Update heights
  x->height = maximum(height(x->left), height(x->right)) + 1;
  y->height = maximum(height(y->left), height(y->right)) + 1;

  // Return new root
  return y;
}

/*
 * Calculate the AVL-balance invariant of a given egl_bstree node
 * If node does not exist, default to 0
 */

static int get_balance(egl_bstree *tree) {
  if (tree == NULL)
    return 0;

  return height(tree->left) - height(tree->right);
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
 * Get the maximum value from a given egl_bstree
 */

static egl_bstree *egl_bstree_maximum(struct egl_bstree *tree) {
  if (tree == NULL) {
    return NULL;
  } else if (tree->left != NULL) {
    return tree->left->maximum(tree->left);
  }
  return tree;
}

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
 * Returns the added node as an egl_bstree
 * Auto-balances the AVL-tree with rotations
 */

static struct egl_bstree *egl_bstree_add(struct egl_bstree *tree, void *i,
                                         int (*compare)(const void *a,
                                                        const void *b)) {
  if (tree == NULL)
    return egl_bstree_new(i);

  if (compare(i, tree->value) < 0) {
    tree->left = egl_bstree_add(tree->left, i, compare);
  } else if (compare(i, tree->value) > 0) {
    tree->right = egl_bstree_add(tree->right, i, compare);
  } else {
    return tree;
  }

  tree->height = maximum(height(tree->left), height(tree->right)) + 1;

  int balance = get_balance(tree);

  if (balance > 1 && compare(i, tree->left->value) < 0) {
    return right_rotate(tree);
  }

  if (balance < -1 && compare(i, tree->right->value) > 0) {
    return left_rotate(tree);
  }

  if (balance > 1 && compare(i, tree->left->value) > 0) {
    tree->left = left_rotate(tree->left);
    return right_rotate(tree);
  }

  if (balance < -1 && compare(i, tree->right->value) < 0) {
    tree->right = right_rotate(tree->right);
    return left_rotate(tree);
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

static egl_bstree *egl_bstree_remove(struct egl_bstree *tree, void *value,
                                     int (*compare)(const void *a,
                                                    const void *b)) {
  if (tree == NULL)
    return tree;

  if (compare(value, tree->value) < 0)
    tree->left = egl_bstree_remove(tree->left, value, compare);

  else if (compare(value, tree->value) > 0)
    tree->right = egl_bstree_remove(tree->right, value, compare);

  else {
    if ((tree->left == NULL) || (tree->right == NULL)) {
      egl_bstree *temp = tree->left ? tree->left : tree->right;

      if (temp == NULL) {
        temp = tree;
        tree = NULL;
      } else {
        tree = temp;
      }
    } else {
      egl_bstree *temp = egl_bstree_minimum(tree->right);

      tree->value = temp->value;

      tree->right = egl_bstree_remove(tree->right, temp->value, compare);
    }
  }

  if (tree == NULL)
    return tree;

  // Update the balance factor of each node and
  // balance the tree
  tree->height = 1 + maximum(height(tree->left), height(tree->right));

  int balance = get_balance(tree);
  if (balance > 1 && get_balance(tree->left) >= 0)
    return right_rotate(tree);

  if (balance > 1 && get_balance(tree->left) < 0) {
    tree->left = left_rotate(tree->left);
    return right_rotate(tree);
  }

  if (balance < -1 && get_balance(tree->right) <= 0)
    return left_rotate(tree);

  if (balance < -1 && get_balance(tree->right) > 0) {
    tree->right = right_rotate(tree->right);
    return left_rotate(tree);
  }

  return tree;
}

/*
 * Initialise a new egl_bstree
 * Set the root value to the specified value
 * Default height to 1
 */

static egl_bstree *egl_bstree_init(egl_bstree *tree, void *val) {
  if (tree == NULL) {
    return NULL;
  }
  tree->value = val;
  tree->height = 1;
  tree->left = NULL;
  tree->right = NULL;
  tree->add = &egl_bstree_add;
  tree->free = &egl_bstree_free;
  tree->find = &egl_bstree_find;
  tree->remove = &egl_bstree_remove;
  tree->minimum = &egl_bstree_minimum;
  tree->maximum = &egl_bstree_maximum;
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
