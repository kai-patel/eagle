#include "egl_matrix.h"
#include <stdlib.h>

/*
 * Free a given egl_matrix and its elements
 */

void egl_matrix_free(struct egl_matrix *a) { return; }

/*
 * Add two M-by-N matrices
 */

static struct egl_matrix *egl_matrix_add(struct egl_matrix *a,
                                         struct egl_matrix *b) {
  return NULL;
}

/*
 * Calculate the element-wise addition of a constant to a given matrix
 */

static struct egl_matrix *egl_matrix_elem_add(struct egl_matrix *a, double k) {
  return NULL;
}

/*
 * Scale a given matrix by a constant
 */

static struct egl_matrix *egl_matrix_scale(struct egl_matrix *a, double c) {
  return NULL;
}

/*
 * Transpose a given matrix
 */

static struct egl_matrix *egl_matrix_transpose(struct egl_matrix *a) {
  return NULL;
}

/*
 * Calculate the matrix product of an M-by-N matrix and a N-by-P matrix
 */

static struct egl_matrix *egl_matrix_mul(struct egl_matrix *a,
                                         struct egl_matrix *b) {
  return NULL;
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

static struct egl_matrix *egl_matrix_fill(struct egl_matrix *a) { return NULL; }

/*
 * Sets all values in a given matrix to zero
 */

static struct egl_matrix *egl_matrix_zero(struct egl_matrix *a) { return NULL; }

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

static double egl_matrix_trace(struct egl_matrix *a) { return 0.0; }

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
