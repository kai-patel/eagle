#ifndef EGL_VECTOR_H
#define EGL_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EGL_INTERNAL_VECTOR_INITIAL_CAPACITY 32

typedef struct egl_vector {
  void **items;
  uint32_t capacity;
  uint32_t length;
  void *(*add)(struct egl_vector *, void *const);
  void (*free)(struct egl_vector *);
  struct egl_vector *(*resize)(struct egl_vector *, uint32_t);
} egl_vector;

egl_vector *egl_vector_new(void);

#endif
