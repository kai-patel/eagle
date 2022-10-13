#include "../src/libeagle.h"
#include "../vendor/Unity/unity.h"
#include <stdlib.h>

egl_bstree *tree;
int *initial_val;

int compare(const void *a, const void *b) { return *((int *)a) - *((int *)b); }

void setUp(void) {
  initial_val = malloc(sizeof(int));
  *initial_val = 0;
  tree = egl_bstree_new(initial_val);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree, "Expected new tree to be created");
  TEST_ASSERT_EQUAL_INT(0, *(int *)tree->value);
}

void tearDown(void) { tree->free(tree); }

/*
 * Create a tree for testing
 * Assumes that a tree with initial value 0 has already been created
 * Assumes that "tree" memory will be freed correctly
 * A balanced version of the tree (as the library is meant to implement) is
 * shown below
 *       3
 *     /   \
 *    1     5
 *   / \   / \
 *  0   2 4   6
 * Fail if error in adding to tree
 */

void createTestTree(void) {
  for (int i = 1; i <= 6; i++) {
    int *val = malloc(sizeof(int));
    *val = i;
    TEST_ASSERT_NOT_NULL_MESSAGE(tree->add(tree, val, compare),
                                 "Expected to be able to add to tree");
  }
}

void test_add(void) {
  createTestTree();

  TEST_ASSERT_NOT_NULL_MESSAGE(tree, "Expected tree to exist");
  TEST_ASSERT_EQUAL_INT(3, *(int *)tree->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left, "Expected node w/ value 1 to exist");
  TEST_ASSERT_EQUAL_INT(1, *(int *)tree->left->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left->left,
                               "Expected node w/ value 0 to exist");
  TEST_ASSERT_EQUAL_INT(0, *(int *)tree->left->left->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left->right,
                               "Expected node w/ value 2 to exist");
  TEST_ASSERT_EQUAL_INT(2, *(int *)tree->left->right->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->right,
                               "Expected node w/ value 5 to exist");
  TEST_ASSERT_EQUAL_INT(5, *(int *)tree->right->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->right->left,
                               "Expected node w/ value 4 to exist");
  TEST_ASSERT_EQUAL_INT(4, *(int *)tree->right->left->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->right->right,
                               "Expected node w/ value 6 to exist");
  TEST_ASSERT_EQUAL_INT(6, *(int *)tree->right->right->value);
}

void test_remove(void) {
  createTestTree();

  int *to_remove = malloc(sizeof(int));
  *to_remove = 5;

  TEST_ASSERT_NOT_NULL_MESSAGE(tree->remove(tree, to_remove, compare),
                               "Expected to be able to remove from tree");
  TEST_ASSERT_NOT_NULL_MESSAGE(tree, "Expected tree to exist");
  TEST_ASSERT_EQUAL_INT(3, *(int *)tree->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left, "Expected node w/ value 1 to exist");
  TEST_ASSERT_EQUAL_INT(1, *(int *)tree->left->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left->left,
                               "Expected node w/ value 0 to exist");
  TEST_ASSERT_EQUAL_INT(0, *(int *)tree->left->left->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->left->right,
                               "Expected node w/ value 2 to exist");
  TEST_ASSERT_EQUAL_INT(2, *(int *)tree->left->right->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->right,
                               "Expected node w/ value 4 to exist");
  TEST_ASSERT_EQUAL_INT(4, *(int *)tree->right->value);
  TEST_ASSERT_NOT_NULL_MESSAGE(tree->right->right,
                               "Expected node w/ value 6 to exist");
  TEST_ASSERT_EQUAL_INT(6, *(int *)tree->right->right->value);
}

void test_find(void) {
  int *to_find = malloc(sizeof(int));
  *to_find = 3;

  createTestTree();

  egl_bstree *found = tree->find(tree, to_find, compare);

  TEST_ASSERT_NOT_NULL_MESSAGE(found, "Expected a node to be found");
  TEST_ASSERT_EQUAL_INT(*to_find, *(int *)found->value);
  TEST_ASSERT_NULL_MESSAGE(
      tree->find(tree, NULL, compare),
      "Expected NULL to be returned, as NULL does not exist");
  free(to_find);
}

void test_minimum(void) {
  for (int i = 1; i < 5; i++) {
    int *val = malloc(sizeof(int));
    *val = i;
    tree->add(tree, val, compare);
  }
  TEST_ASSERT_EQUAL_INT(0, *(int *)(tree->minimum(tree)->value));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_remove);
  RUN_TEST(test_find);
  RUN_TEST(test_minimum);
  return UNITY_END();
}
