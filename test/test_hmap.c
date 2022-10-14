#include "../src/libeagle.h"
#include "../vendor/Unity/unity.h"
#include <stdlib.h>

egl_hmap *map;

void setUp(void) {}

void tearDown(void) {}

void test_add(void) {}

void test_remove(void) {}

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
