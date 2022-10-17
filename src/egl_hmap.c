#include "egl_hmap.h"
#include <stdlib.h>

#ifndef EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY
#define EAGLE_INTERNAL_HMAP_INITIAL_CAPACITY 509
#endif

/*
 * Check whether a given integer is prime
 * Time complexity O(sqrt(n))
 * Avoids using early break
 */

static bool is_prime(uint64_t n) {
  if (n <= 1)
    return false;
  if (n == 2)
    return true;

  for (uint64_t i = 2; i < n; i++) {
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

uint64_t hash(void *k, uint64_t capacity) {
  uint64_t m = ((uint64_t)k * (uint64_t)2654435761) % ((uint64_t)1 << 32);
  return m % capacity;
}

/*
 * Helper function to free the internals of an egl_hmap_bucket, but not the
 * struct itself
 */

static void free_bucket(struct egl_hmap_bucket *bucket) {
  if (bucket == NULL)
    return;
  if (bucket->key == bucket->value) {
    free(bucket->key);
  } else {
    if (bucket->key != NULL)
      free(bucket->key);
    if (bucket->value != NULL)
      free(bucket->value);
  }
}

/*
 * Free all keys, values, and underlying data structures for a given egl_hmap
 */

static void egl_hmap_free(struct egl_hmap *map) {
  if (map == NULL)
    return;

  if (map->array == NULL)
    return;

  for (uint64_t i = 0; i < map->capacity; i++) {
    free_bucket(&map->array[i]);
  }

  free(map->array);
  free(map);
}

/*
 * Add a key and its value to a given egl_hmap
 * Returns the added key
 * Returns NULL if an error occurred
 */

static void *egl_hmap_add(struct egl_hmap *map, void *key, void *value,
                          int (*compare)(const void *, const void *)) {
  uint64_t index = hash(key, map->capacity);

  // Error if no space left
  if (map->size == map->capacity) {
    return NULL;
  }

  // Linear probing
  while (map->array[index].key != NULL) {
    if (compare(map->array[index].key, key) == 0) {
      map->array[index].value = value;
      return map->array[index].key;
    }

    index = (index + 1) % map->capacity;
  }

  map->array[index].key = key;
  map->array[index].value = value;

  map->size++;
  return map->array[index].key;
}

/*
 * Remove a key and its value from a given egl_hmap
 * Removals are marked with a tombstone
 * Tombstones are a buckets with key NULL
 * Returns the egl_hmap on success
 * Returns NULL if an error occurred
 */

static egl_hmap *egl_hmap_remove(struct egl_hmap *map, void *key,
                                 int (*compare)(const void *, const void *)) {

  uint64_t index = hash(key, map->capacity);

  while (map->array[index].key != NULL) {
    if (compare(map->array[index].key, key) == 0) {
      map->array[index].key = NULL;
      map->size--;

      uint64_t i = index;
      while (map->array[i].key != NULL) {
        if (hash(map->array[i].key, map->capacity) <= index)
          break;
        i = (i + 1) % map->capacity;
      }

      // Element "i" is now either empty or needs to be moved
      map->array[index] = map->array[i];
      map->array[i].key = NULL;
      map->array[i].value = NULL;

      return map;
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
  for (uint64_t i = 0; i < map->capacity; i++) {
    if (map->array[i].key != NULL) {
      if (compare(map->array[i].value, value) == 0) {
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
  for (uint64_t i = 0; i < map->capacity; i++) {
    if (map->array[i].key != NULL) {
      if (compare(map->array[i].key, key) == 0) {
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

  uint64_t index = hash(key, map->capacity);

  while (map->array[index].key != NULL) {
    if (compare(map->array[index].key, key) == 0) {
      return map->array[index].value;
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

static egl_hmap *egl_hmap_init(struct egl_hmap *map, uint64_t capacity) {
  if (map == NULL)
    return NULL;

  map->size = 0;
  map->array = malloc(capacity * sizeof(struct egl_hmap_bucket));

  for (uint64_t i = 0; i < capacity; i++) {
    map->array[i].key = NULL;
    map->array[i].value = NULL;
  }

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

egl_hmap *egl_hmap_new(uint64_t capacity) {
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
