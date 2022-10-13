#include "../src/libeagle.h"
#include "../vendor/Unity/unity.h"
#include <stdlib.h>

egl_vector *vec;

void setUp(void) {
  vec = egl_vector_new();
  TEST_ASSERT_NOT_NULL_MESSAGE(vec, "Expected new vector to be created");
  TEST_ASSERT_EMPTY_MESSAGE(vec->items, "Expected new vector to be empty");
}

void tearDown(void) { vec->free(vec); }

void test_add(void) {
  for (int i = 0; i < 1024; i++) {
    int *item = malloc(sizeof(int));
    if (item != NULL)
      *item = i + 1;
    if (vec)
      vec->add(vec, (void *)item);
  }

  for (size_t i = 0; i < vec->length; i++) {
    TEST_ASSERT_EQUAL_INT(i + 1, *(int *)(vec->items[i]));
  }
}

void test_length(void) {
  for (int i = 0; i < 1024; i++) {
    int *item = malloc(sizeof(int));
    if (item != NULL)
      *item = i + 1;
    if (vec)
      vec->add(vec, (void *)item);
  }

  TEST_ASSERT_EQUAL_INT(1024, vec->length);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_length);
  return UNITY_END();
}
