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
}

void test_contains_key(void) {}

void test_contains_value(void) {}

void test_get(void) {}

void test_to_array(void) {}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_remove);
  RUN_TEST(test_contains_key);
  RUN_TEST(test_contains_value);
  RUN_TEST(test_get);
  RUN_TEST(test_to_array);
  return UNITY_END();
}
