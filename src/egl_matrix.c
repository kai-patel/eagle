#include "egl_matrix.h"
#include <stdlib.h>

/*
 * Free a given egl_matrix and its elements
 */

void egl_matrix_free(struct egl_matrix *a) { return; }

/*
 * Add two M-by-N matrices
 * Returns NULL if the matrices are different sizes
 */

static struct egl_matrix *egl_matrix_add(struct egl_matrix *a,
                                         struct egl_matrix *b) {
  if (a->m != b->m || a->n != b->n)
    return NULL;

  for (size_t i = 0; i < a->n * a->m; i++) {
    a->elements[i] += b->elements[i];
  }

  return a;
}

/*
 * Calculate the element-wise addition of a constant to a given matrix
 */

static struct egl_matrix *egl_matrix_elem_add(struct egl_matrix *a, double k) {
  for (size_t i = 0; i < a->m * a->n; i++) {
    a->elements[i] += k;
  }

  return a;
}

/*
 * Scale a given matrix by a constant
 */

static struct egl_matrix *egl_matrix_scale(struct egl_matrix *a, double c) {
  for (size_t i = 0; i < a->m * a->n; i++) {
    a->elements[i] *= c;
  }

  return a;
}

/*
 * Transpose a given matrix
 */

static struct egl_matrix *egl_matrix_transpose(struct egl_matrix *a) {
  return NULL;
}

/*
 * Calculate the matrix product of an M-by-N matrix and a N-by-P matrix
 * Returns NULL if the matrices have incompatible sizes
 */

static struct egl_matrix *egl_matrix_mul(struct egl_matrix *a,
                                         struct egl_matrix *b) {
  if (a->n != b->m)
    return NULL;

  egl_matrix *res = egl_matrix_new(a->m, b->n);

  for (size_t i = 0; i < a->m; i++) {
    for (size_t j = 0; j < b->n; j++) {
      double sum = 0.0;
      for (size_t k = 0; k < a->n; k++) {
        sum += a->elements[k + i * a->n] * b->elements[j + k * b->n];
      }
      res->elements[j + i * res->n] = sum;
    }
  }

  return res;
}

/*
 * Calculate the inverse of a given matrix
 * Returns NULL if an error occurred
 */

static struct egl_matrix *egl_matrix_inverse(struct egl_matrix *a) {
  return NULL;
}

/*
 * Fills a given matrix with a specified value
 */

static struct egl_matrix *egl_matrix_fill(struct egl_matrix *a, double k) {
  for (size_t i = 0; i < a->m * a->n; i++) {
    a->elements[i] = k;
  }

  return a;
}

/*
 * Sets all values in a given matrix to zero
 */

static struct egl_matrix *egl_matrix_zero(struct egl_matrix *a) {
  for (size_t i = 0; i < a->m * a->n; i++) {
    a->elements[i] = 0.0;
  }

  return a;
}

/*
 * Sum the values of a matrix along a specified axis
 */

static struct egl_matrix *egl_matrix_sum(size_t index,
                                         enum EGL_MATRIX_AXIS axis) {
  return NULL;
}

/*
 * Calculate the trace of a given matrix
 */

static double egl_matrix_trace(struct egl_matrix *a) {
  double total = 0.0;
  for (size_t i = 0; i < a->m; i++) {
    for (size_t j = 0; j < a->n; j++) {
      if (i == j)
        total += a->elements[j + i * a->n];
    }
  }

  return total;
}

/*
 * Calculate the determinant of a given matrix
 */

static double egl_matrix_det(struct egl_matrix *a) { return 0.0; }

/*
 * Initialise a newly created egl_matrix of given size
 * Allocate memory for elements and set to 0.0
 */

static egl_matrix *egl_matrix_init(egl_matrix *a, size_t m, size_t n) {
  if (a == NULL)
    return NULL;

  a->elements = calloc(m * n, sizeof(double));
  a->m = m;
  a->n = n;
  a->add = &egl_matrix_add;
  a->elem_add = &egl_matrix_elem_add;
  a->scale = &egl_matrix_scale;
  a->transpose = &egl_matrix_transpose;
  a->mul = &egl_matrix_mul;
  a->inverse = &egl_matrix_inverse;
  a->fill = &egl_matrix_fill;
  a->zero = &egl_matrix_zero;
  a->sum = &egl_matrix_sum;
  a->trace = &egl_matrix_trace;
  a->det = &egl_matrix_det;
  a->free = &egl_matrix_free;
  return a;
}

/*
 * Create a new matrix with specified dimensions
 * Returns NULL if an error occurred
 */

egl_matrix *egl_matrix_new(size_t m, size_t n) {
  egl_matrix *a = malloc(sizeof(egl_matrix));
  return egl_matrix_init(a, m, n);
}
