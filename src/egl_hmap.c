#include "egl_hmap.h"
#include <stdint.h>
#include <stdlib.h>

#ifndef EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY
#define EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY 509
#endif

/*
 * Check whether a given integer is prime
 * Time complexity O(sqrt(n))
 * Avoids using early break
 */

static bool is_prime(size_t n) {
  if (n <= 1)
    return false;
  if (n == 2)
    return true;

  for (size_t i = 2; i < n; i++) {
    if (n % i == 0) {
      return false;
    }

    if (i * i >= n) {
      break;
    }
  }

  return true;
}

/*
 * Compute the hash of a given pointer
 * Uses Knuth's Mutiplicative Method
 */

static size_t hash(void *k, size_t capacity) {
  uint64_t m = ((uint64_t)k * 2654435761) % ((uint64_t)1 << 32);
  return m % capacity;
}

/*
 * Free all keys, values, and underlying data structures for a given egl_hmap
 */

static void egl_hmap_free(struct egl_hmap *map) {
  for (size_t i = 0; i < map->capacity; i++) {
    free(map->array[i]->key);
    free(map->array[i]->value);
    free(map->array[i]);
  }

  free(map);
}

/*
 * Add a key and its value to a given egl_hmap
 * Returns the added key
 * Returns NULL if an error occurred
 */

static void *egl_hmap_add(struct egl_hmap *map, void *key, void *value,
                          int (*compare)(const void *, const void *)) {
  size_t index = hash(key, map->capacity);

  // Error if no space left
  if (map->size == map->capacity) {
    return NULL;
  }

  // Linear probing
  while (map->array[index] != NULL) {
    if (compare(map->array[index]->key, key) == 0) {
      goto add;
    }
    index = (index + 1) % map->capacity;
  }

  map->array[index] = malloc(sizeof(struct egl_hmap_bucket));

add:
  map->array[index]->key = key;
  map->array[index]->value = value;

  map->size++;
  return map->array[index]->key;
}

/*
 * Remove a key and its value from a given egl_hmap
 * Removals are marked with a tombstone
 * Tombstones are a NULL
 * Returns the removed key
 * Returns NULL if an error occurred
 */

static void *egl_hmap_remove(struct egl_hmap *map, void *key,
                             int (*compare)(const void *, const void *)) {

  size_t index = hash(key, map->capacity);

  while (map->array[index] != NULL) {

    if (compare(map->array[index]->key, key) == 0) {
      void *ret = map->array[index]->key;
      map->array[index]->key = NULL;
      return ret;
    }

    index = (index + 1) % map->capacity;
  }

  return NULL;
}

/*
 * Test whether a given value exists in an egl_hmap
 */

static bool egl_hmap_contains_value(struct egl_hmap *map, void *value,
                                    int (*compare)(const void *,
                                                   const void *)) {
  for (size_t i = 0; i < map->capacity; i++) {
    if (map->array[i] != NULL) {
      if (compare(map->array[i]->value, value) == 0) {
        return true;
      }
    }
  }
  return false;
}

/*
 * Test whether a given key exists in an egl_hmap
 */

static bool egl_hmap_contains_key(struct egl_hmap *map, void *key,
                                  int (*compare)(const void *, const void *)) {
  for (size_t i = 0; i < map->capacity; i++) {
    if (map->array[i] != NULL) {
      if (compare(map->array[i]->key, key) == 0) {
        return true;
      }
    }
  }
  return false;
}

/*
 * Get a value from an egl_hmap, given a key
 * Returns NULL if the value is not found
 * NB: Use the "contains_key" function to check whether the value is NULL, or
 * whether no entry was found
 */

static void *egl_hmap_get(struct egl_hmap *map, void *key,
                          int (*compare)(const void *, const void *)) {

  size_t index = hash(key, map->capacity);

  while (map->array[index] != NULL) {
    if (compare(map->array[index]->key, key) == 0) {
      return map->array[index]->value;
    }
    index = (index + 1) % map->capacity;
  }

  return NULL;
}

/*
 * Initialise a newly created egl_hmap
 * Set initial size to 0
 * Allocate memory for "capacity" number of elements
 */

static egl_hmap *egl_hmap_init(struct egl_hmap *map, size_t capacity) {
  if (map == NULL)
    return NULL;

  map->size = 0;
  map->array = calloc(capacity, sizeof(struct egl_hmap_bucket *));
  map->capacity = capacity;
  map->add = &egl_hmap_add;
  map->remove = &egl_hmap_remove;
  map->contains_key = &egl_hmap_contains_key;
  map->contains_value = &egl_hmap_contains_value;
  map->get = &egl_hmap_get;
  map->free = &egl_hmap_free;
  return map;
}

/*
 * Creates a new, empty egl_hmap of specified capacity
 * If capacity is 0, use EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY as default
 * Panics if the specified capacity is not prime
 * Returns NULL if an error occurred, or if the capacity is not prime
 */

egl_hmap *egl_hmap_new(size_t capacity) {
  if (capacity == 0) {
    capacity = EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY;
  }

  if (!is_prime(capacity)) {
    return NULL;
  }

  egl_hmap *map = malloc(sizeof(egl_hmap));
  map = egl_hmap_init(map, capacity);
  return map;
}
