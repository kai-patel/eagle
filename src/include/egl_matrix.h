#ifndef EGL_MATRIX_H
#define EGL_MATRIX_H

enum EGL_MATRIX_AXIS { ROW, COLUMN, DIAGONAL, SECONDARY_DIAGONAL };

typedef struct egl_matrix {
  double *elements;
  size_t m;
  size_t n;
  struct egl_matrix *(*add)(struct egl_matrix *, struct egl_matrix *);
  struct egl_matrix *(*elem_add)(struct egl_matrix *, double);
  struct egl_matrix *(*scale)(struct egl_matrix *, double);
  struct egl_matrix *(*transpose)(struct egl_matrix *);
  struct egl_matrix *(*mul)(struct egl_matrix *, struct egl_matrix *);
  struct egl_matrix *(*inverse)(struct egl_matrix *);
  struct egl_matrix *(*fill)(struct egl_matrix *, double);
  struct egl_matrix *(*zero)(struct egl_matrix *);
  struct egl_LU (*lu_decompose)(struct egl_matrix *);
  double (*sum)(struct egl_matrix *, size_t, enum EGL_MATRIX_AXIS);
  double (*trace)(struct egl_matrix *);
  double (*det)(struct egl_matrix *);
  void (*free)(struct egl_matrix *);
} egl_matrix;

/*
 * Struct for results of LU decomposition operations
 */

typedef struct egl_LU {
  struct egl_matrix *L;
  struct egl_matrix *U;
} egl_LU;

egl_matrix *egl_matrix_new(size_t, size_t);

#endif
