#include "libeagle.h"
#include <unity.h>
#include <stdlib.h>

egl_llist *llist;

int compare(const void *a, const void *b) { return *((int *)a) - *((int *)b); }

void setUp(void) {
  llist = egl_llist_new();
  TEST_ASSERT_NOT_NULL_MESSAGE(llist, "Expected new llist to be created");
  TEST_ASSERT_NULL_MESSAGE(llist->head,
                           "Expected newly created llist to have no head");
}

void tearDown(void) { llist->free(llist); }

void test_add(void) {
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    llist->add(llist, (void *)new_val);
  }

  egl_llist_node *curr = llist->head;
  int i = 0;
  while (curr != NULL) {
    TEST_ASSERT_EQUAL_INT(i + 1, *(int *)curr->value);
    curr = curr->next;
    i++;
  }
}

void test_length(void) {
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    llist->add(llist, (void *)new_val);
  }

  egl_llist_node *curr = llist->head;
  int i = 0;
  while (curr != NULL) {
    i++;
    curr = curr->next;
  }

  TEST_ASSERT_EQUAL_INT(16, i);
}

void test_find(void) {
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    llist->add(llist, (void *)new_val);
  }

  int *val_to_find = malloc(sizeof(int));
  *val_to_find = 2;

  egl_llist_node *found_node = llist->find(llist, (void *)val_to_find, compare);
  TEST_ASSERT_EQUAL(llist->head->next, found_node);
  free(val_to_find);
}

void test_reverse(void) {
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    llist->add(llist, (void *)new_val);
  }

  llist->reverse(llist); // Reverse the list
  llist->reverse(llist); // "Unreverse" it

  egl_llist_node *curr = llist->head;
  int i = 0;
  while (curr != NULL) {
    TEST_ASSERT_EQUAL_INT(i + 1, *(int *)curr->value);
    curr = curr->next;
    i++;
  }
}

void test_delete_p(void) {
  void *to_delete;
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    if (*new_val == 2) {
      to_delete = new_val;
    }
    llist->add(llist, (void *)new_val);
  }

  llist->delete_p(llist, to_delete);
  TEST_ASSERT_EQUAL_INT(3, *(int *)llist->head->next->value);
}

void test_delete_v(void) {
  void *to_delete;
  for (int i = 0; i < 16; i++) {
    int *new_val = malloc(sizeof(int));
    *new_val = i + 1;
    if (*new_val == 2) {
      to_delete = new_val;
    }
    llist->add(llist, (void *)new_val);
  }

  llist->delete_v(llist, to_delete, compare);
  TEST_ASSERT_EQUAL_INT(3, *(int *)llist->head->next->value);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_length);
  RUN_TEST(test_find);
  RUN_TEST(test_reverse);
  RUN_TEST(test_delete_p);
  RUN_TEST(test_delete_v);
  return UNITY_END();
}
