#include "../src/libeagle.h"
#include "../vendor/Unity/unity.h"
#include <stdlib.h>

egl_hmap *map;

int compare(const void *a, const void *b) { return *((int *)a) - *((int *)b); }

void setUp(void) {
  map = egl_hmap_new(0);

  TEST_ASSERT_NOT_NULL_MESSAGE(map, "Expected new egl_hmap to be created");
}

void tearDown(void) { map->free(map); }

void test_add(void) {
  for (int i = 0; i < 256; i++) {
    int *to_add = malloc(sizeof(int));
    *to_add = i;
    void *res = map->add(map, to_add, to_add, compare);
    TEST_ASSERT_NOT_NULL_MESSAGE(res, "Expected to be able to add");
    TEST_ASSERT_EQUAL_PTR(to_add, res);
  }
}

void test_remove(void) {
  test_add();

  int *to_remove = malloc(sizeof(int));
  *to_remove = 5;

  void *res = map->remove(map, to_remove, compare);
  TEST_ASSERT_NOT_NULL_MESSAGE(res, "Expected to be able to remove");

  *to_remove = 256;
  res = map->remove(map, to_remove, compare);
  TEST_ASSERT_NULL_MESSAGE(
      res, "Expected to not be able to remove element not in map");

  free(to_remove);
}

void test_contains_key(void) {
  test_add();

  int *to_find = malloc(sizeof(int));
  *to_find = 24;

  TEST_ASSERT_TRUE_MESSAGE(map->contains_key(map, to_find, compare),
                           "Expected to be able to find key 24 in map");

  *to_find = 1024;
  TEST_ASSERT_FALSE_MESSAGE(map->contains_key(map, to_find, compare),
                            "Expected not to find key 1024 in map");

  free(to_find);
}

void test_contains_value(void) {
  test_add();

  int *to_find = malloc(sizeof(int));
  *to_find = 24;

  TEST_ASSERT_TRUE_MESSAGE(map->contains_value(map, to_find, compare),
                           "Expected to be able to find value 24 in map");

  *to_find = 1024;
  TEST_ASSERT_FALSE_MESSAGE(map->contains_value(map, to_find, compare),
                            "Expected not to find value 1024 in map");

  free(to_find);
}

void test_get(void) {
  test_add();

  int *to_get = malloc(sizeof(int));
  *to_get = 30;

  void *res = map->get(map, to_get, compare);
  TEST_ASSERT_EQUAL_INT(*to_get, *(int *)res);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_remove);
  RUN_TEST(test_contains_key);
  RUN_TEST(test_contains_value);
  RUN_TEST(test_get);
  return UNITY_END();
}
