#include "egl_matrix.h"
#include <stdlib.h>

static struct egl_LU egl_matrix_decompose(struct egl_matrix *a) {
  size_t n = a->m;
  egl_matrix *L = egl_matrix_new(n, n);
  egl_matrix *U = egl_matrix_new(n, n);

  for (size_t k = 0; k < n; k++) {
    U->elements[k + k * n] = a->elements[k + k * n];
    for (size_t i = k; i < n; i++) {
      L->elements[k + i * n] = a->elements[k + i * n] / U->elements[k + k * n];
      U->elements[i + k * n] = a->elements[i + k * n];
    }
    for (size_t i = k; i < n; i++) {
      for (size_t j = k; j < n; j++) {
        a->elements[j + i * n] =
            a->elements[j + i * n] -
            (L->elements[k + i * n] * U->elements[j + k * n]);
      }
    }
  }

  egl_LU pair = {.L = L, .U = U};
  return pair;
}

/*
 * Free a given egl_matrix and its elements
 */

static void egl_matrix_free(struct egl_matrix *a) {
  if (a == NULL)
    return;
  if (a->elements != NULL)
    free(a->elements);
  free(a);
}

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
 * Returns NULL if an error occurred
 */

static struct egl_matrix *egl_matrix_transpose(struct egl_matrix *a) {
  egl_matrix *res = egl_matrix_new(a->n, a->m);

  for (size_t j = 0; j < a->n; j++) {
    for (size_t i = 0; i < a->m; i++) {
      res->elements[i + j * res->n] = a->elements[j + i * a->n];
    }
  }

  return res;
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
 * Returns NULL if the matrix is not square, or an error occurred
 */

static struct egl_matrix *egl_matrix_inverse(struct egl_matrix *a) {
  if (a->m != a->n)
    return NULL;

  size_t n = a->m;
  egl_matrix *res = egl_matrix_new(n, 2 * n);
  if (res == NULL)
    return NULL;

  res = res->zero(res);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      res->elements[j + i * 2 * n] = a->elements[j + i * n];
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i == j) {
        res->elements[(j + n) + i * 2 * n] = 1.0;
      }
    }
  }

  for (size_t i = 0; i < n; i++) {
    if (res->elements[i + i * 2 * n] == 0) {
      res->free(res);
      return NULL; // Error due to division by zero
    }

    for (size_t j = 0; j < n; j++) {
      if (i != j) {
        double ratio =
            res->elements[i + j * 2 * n] / res->elements[i + i * 2 * n];

        for (size_t k = 0; k < 2 * n; k++) {
          res->elements[k + j * 2 * n] = res->elements[k + j * 2 * n] -
                                         (ratio * res->elements[k + i * 2 * n]);
        }
      }
    }
  }

  for (size_t i = 0; i < n; i++) {
    double divisor = res->elements[i + i * 2 * n];
    for (size_t j = 0; j < 2 * n; j++) {
      res->elements[j + i * 2 * n] = res->elements[j + i * 2 * n] / divisor;
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = n; j < 2 * n; j++) {
      a->elements[(j - n) + i * n] = res->elements[j + i * 2 * n];
    }
  }

  res->free(res);

  return a;
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

static double egl_matrix_sum(struct egl_matrix *a, size_t index,
                             enum EGL_MATRIX_AXIS axis) {
  return 0.0;
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
 * Factorises the given matrix through LU decomposition
 * Returns 0.0 if the matrix is not square, or on error
 */

static double egl_matrix_det(struct egl_matrix *a) {
  if (a->m != a->n)
    return 0.0;

  size_t n = a->m;
  struct egl_LU LU = egl_matrix_decompose(a);
  egl_matrix *L = LU.L;
  egl_matrix *U = LU.U;

  if (L == NULL || U == NULL)
    return 0.0;

  double detU = 1.0;
  double detL = 1.0;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i == j)
        detU *= U->elements[j + i * n];
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i == j)
        detL *= L->elements[j + i * n];
    }
  }

  U->free(U);
  L->free(L);

  return detU * detL;
}

/*
 * Initialise a newly created egl_matrix of given size
 * Allocate memory for elements and set to 0.0
 */

static egl_matrix *egl_matrix_init(egl_matrix *a, size_t m, size_t n) {
  if (a == NULL)
    return NULL;

  a->elements = calloc(m * n, sizeof(double));

  if (a->elements == NULL) {
    free(a);
    return NULL;
  }

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
  a->lu_decompose = &egl_matrix_decompose;
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
