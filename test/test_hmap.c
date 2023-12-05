#include "libeagle.h"
#include <unity.h>
#include <stdint.h>
#include <stdlib.h>

egl_hmap *map;

int compare(const void *a, const void *b) { return *((int *)a) - *((int *)b); }

void setUp(void) {
  map = egl_hmap_new(0);

  TEST_ASSERT_NOT_NULL_MESSAGE(map, "Expected new egl_hmap to be created");
}

void tearDown(void) {
  map->free(map);
  return;
}

void createTestMap(void) {
  for (int i = 0; i < 32; i++) {
    int *to_add = malloc(sizeof(int));
    *to_add = i;
    void *res = map->add(map, to_add, to_add, compare);
    TEST_ASSERT_NOT_NULL_MESSAGE(res, "Expected to be able to add");
    TEST_ASSERT_EQUAL_PTR(to_add, res);
  }
}

void test_add(void) { createTestMap(); }

void test_remove(void) {
  createTestMap();

  int *to_remove = NULL;

  for (uint64_t i = 0; i < map->capacity; i++) {
    if (map->array[i].key != NULL) {
      to_remove = map->array[i].key;
      break;
    }
  }

  TEST_ASSERT_NOT_NULL_MESSAGE(
      to_remove, "Expected to find at least one non-null element in map");

  if (to_remove == NULL)
    TEST_ABORT();

  egl_hmap *res = map->remove(map, to_remove, compare);
  TEST_ASSERT_NOT_NULL_MESSAGE(res, "Expected to be able to remove");

  res = map->remove(map, NULL, compare);
  TEST_ASSERT_NULL_MESSAGE(res, "Expected to not be able to NULL element");
}

void test_contains_key(void) {
  createTestMap();

  int *to_find = malloc(sizeof(int));
  *to_find = 5;

  TEST_ASSERT_TRUE_MESSAGE(map->contains_key(map, to_find, compare),
                           "Expected to be able to find key 24 in map");

  *to_find = 1024;
  TEST_ASSERT_FALSE_MESSAGE(map->contains_key(map, to_find, compare),
                            "Expected not to find key 1024 in map");

  free(to_find);
}

void test_contains_value(void) {
  createTestMap();

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
  createTestMap();

  int *to_get = NULL;

  for (uint64_t i = 0; i < map->capacity; i++) {
    if (map->array[i].key != NULL) {
      to_get = map->array[i].key;
      break;
    }
  }

  TEST_ASSERT_NOT_NULL_MESSAGE(to_get,
                               "Expected to find at least one non-null value");
  if (to_get == NULL)
    TEST_ABORT();

  void *res = map->get(map, to_get, compare);
  TEST_ASSERT_NOT_NULL_MESSAGE(res, "Expected to be able to get");
  TEST_ASSERT_EQUAL_INT(*to_get, *(int *)res);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  if (TEST_PROTECT())
    RUN_TEST(test_remove);
  RUN_TEST(test_contains_key);
  RUN_TEST(test_contains_value);
  if (TEST_PROTECT())
    RUN_TEST(test_get);
  return UNITY_END();
}
