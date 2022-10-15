#ifndef EGL_HMAP_H
#define EGL_HMAP_H

#include "egl_vector.h"
#include <stdbool.h>

/* Internal struct used to represent non-empty buckets
 * Empty buckets are represented as NULL
 */

struct egl_hmap_bucket {
  void *key;
  void *value;
};

typedef struct egl_hmap {
  size_t size;
  size_t capacity;
  void *(*add)(struct egl_hmap *, void *, void *,
               int (*compare)(const void *, const void *));
  void *(*remove)(struct egl_hmap *, void *,
                  int (*compare)(const void *, const void *));
  bool (*contains_key)(struct egl_hmap *, void *,
                       int (*compare)(const void *, const void *));
  bool (*contains_value)(struct egl_hmap *, void *,
                         int (*compare)(const void *, const void *));
  void *(*get)(struct egl_hmap *, void *,
               int (*compare)(const void *, const void *));
  struct egl_hmap_bucket **array;
  void (*free)(struct egl_hmap *);
} egl_hmap;

egl_hmap *egl_hmap_new(size_t);

#endif
