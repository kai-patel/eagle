#ifndef EGL_HMAP_H
#define EGL_HMAP_H

#include "egl_vector.h"
#include <stdbool.h>
#include <stdint.h>

/* Internal struct used to represent non-empty buckets
 * Empty buckets are represented as NULL
 */

struct egl_hmap_bucket {
  void *key;
  void *value;
};

typedef struct egl_hmap {
  uint64_t size;
  uint64_t capacity;
  void *(*add)(struct egl_hmap *, void *, void *,
               int (*compare)(const void *, const void *));
  struct egl_hmap *(*remove)(struct egl_hmap *, void *,
                             int (*compare)(const void *, const void *));
  bool (*contains_key)(struct egl_hmap *const, void *const,
                       int (*compare)(const void *, const void *));
  bool (*contains_value)(struct egl_hmap *const, void *const,
                         int (*compare)(const void *, const void *));
  void *(*get)(struct egl_hmap *, void *const,
               int (*compare)(const void *, const void *));
  struct egl_hmap_bucket *array;
  void (*free)(struct egl_hmap *);
} egl_hmap;

egl_hmap *egl_hmap_new(size_t);
uint64_t hash(void *, uint64_t);

#endif
