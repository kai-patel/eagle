#include "../src/libeagle.h"
#include "../vendor/Unity/unity.h"
#include <assert.h>
#include <stdlib.h>

egl_matrix *mat;

void setUp(void) { mat = egl_matrix_new(4, 5); }

void createTestElements(void) {
  for (size_t i = 0; i < mat->m * mat->n; i++) {
    mat->elements[i] = (double)i;
  }
}

void tearDown(void) { mat->free(mat); }

void test_add(void) {
  createTestElements();
  egl_matrix *mat_b = egl_matrix_new(4, 5);

  for (size_t i = 0; i < mat_b->m * mat_b->n; i++) {
    mat_b->elements[i] = (double)i;
  }

  egl_matrix *res = mat->add(mat, mat_b);

  TEST_ASSERT_NOT_NULL_MESSAGE(
      res, "Expected matrices to have compatible sizes for addition");

  for (size_t i = 0; i < res->m * res->n; i++) {
    TEST_ASSERT_EQUAL_DOUBLE(mat_b->elements[i] * 2, res->elements[i]);
  }
}

void test_elem_add(void) {
  createTestElements();

  egl_matrix *res = mat->elem_add(mat, 5.0);

  for (size_t i = 0; i < res->m * res->n; i++) {
    TEST_ASSERT_EQUAL_DOUBLE((double)i + 5.0, res->elements[i]);
  }
}

void test_scale(void) {
  createTestElements();

  egl_matrix *res = mat->scale(mat, 5.0);

  for (size_t i = 0; i < res->m * res->n; i++) {
    TEST_ASSERT_EQUAL_DOUBLE((double)i * 5.0, res->elements[i]);
  }
}

void test_transpose(void) { TEST_IGNORE(); }

void test_mul(void) {
  createTestElements();
  egl_matrix *mat_b = egl_matrix_new(5, 1);

  for (size_t i = 0; i < mat_b->m * mat_b->n; i++) {
    mat_b->elements[i] = (double)i;
  }

  egl_matrix *res = mat->mul(mat, mat_b);

  double expected[4] = {30.0, 80.0, 130.0, 180.0};

  assert(expected); // Stop the compiler complaining about unused variable

  TEST_ASSERT_NOT_NULL_MESSAGE(
      res, "Expected matrices to have compatible sizes for multiplication");
  TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, res->elements, res->m * res->n);
}

void test_inverse(void) { TEST_IGNORE(); }

void test_fill(void) {
  double filler = 12.0;
  mat->fill(mat, filler);

  TEST_ASSERT_EACH_EQUAL_DOUBLE_MESSAGE(
      filler, mat->elements, mat->m * mat->n,
      "Expected to fill matrix with specified number");
}

void test_zero(void) {
  mat->zero(mat);

  TEST_ASSERT_EACH_EQUAL_DOUBLE_MESSAGE(0.0, mat->elements, mat->m * mat->n,
                                        "Expected every element to be 0.0");
}

void test_sum(void) { TEST_IGNORE(); }

void test_trace(void) {
  createTestElements();

  TEST_ASSERT_EQUAL_DOUBLE(36, mat->trace(mat));
}

void test_det(void) { TEST_IGNORE(); }

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_add);
  RUN_TEST(test_elem_add);
  RUN_TEST(test_scale);
  RUN_TEST(test_transpose);
  RUN_TEST(test_mul);
  RUN_TEST(test_inverse);
  RUN_TEST(test_fill);
  RUN_TEST(test_zero);
  RUN_TEST(test_sum);
  RUN_TEST(test_trace);
  RUN_TEST(test_det);
  return UNITY_END();
}