#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

static void swap(void *a, void *b, size_t size) {
#define min(a, b) ((a) < (b) ? (a) : (b))
  for (char tmp[256]; size; size -= min(size, sizeof(tmp))) {
    memcpy(tmp, a, min(size, sizeof(tmp)));
    memcpy(a,   b, min(size, sizeof(tmp)));
    memcpy(b, tmp, min(size, sizeof(tmp)));
  }
#undef min
}

// macros make the code more natural to read (imo)
#define swap(a, b) swap(a, b, size)
#define cmp(a, b) (f1 ? f1(&a, &b) : f2(&a, &b, arg))

static void sift_down(void *base, size_t size, size_t top, size_t bottom,
    int (*f1)(const void *, const void *),
    int (*f2)(const void *, const void *, void *),
    void *arg) {

  typedef char type[size];
  type *array = (void *)base;

  for (size_t child; top * 2 + 1 <= bottom; top = child) {
    child = top * 2 + 1;
    if (child + 1 <= bottom && cmp(array[child], array[child+1]) < 0) child++;
    if (cmp(array[child], array[top]) < 0) return;
    swap(&array[child], &array[top]);
  }
}

static void actual_qsort(void *base, size_t nmemb, size_t size,
    int (*f1)(const void *, const void *),
    int (*f2)(const void *, const void *, void *),
    void *arg) {

  typedef char type[size];
  type *array = (void *)base;

  // optimize a few cases
  if (nmemb < 2) return;
  if (nmemb == 2) {
    if (cmp(array[0], array[1]) < 0) swap(&array[0], &array[1]);
    return;
  }
  if (nmemb < 10) { // insertion sort
    for (size_t i = 1; i < nmemb; i++)
      for (size_t j = i; j > 0 && cmp(array[j-1], array[j]) > 0; j--)
        swap(&array[j-1], &array[j]);
    return;
  }

#define sift_down(arr, top, bottom) sift_down(arr, size, top, bottom, f1, f2, arg)

  // heap sort
  for (ssize_t top = (nmemb-2) / 2; top >= 0; top--) // signed
    sift_down(array, top, nmemb-1);

  for (size_t i = nmemb-1; i > 0; sift_down(array, 0, --i))
    swap(&array[0], &array[i]);

#undef sift_down
}

#undef swap
#undef cmp

void qsort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *)) {
  actual_qsort(base, nmemb, size, compar, NULL, NULL);
}

void qsort_r(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *, void *), void *arg) {
  actual_qsort(base, nmemb, size, NULL, compar, arg);
}

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *)) {

  typedef char type[size];
  type *array = (void *)base;
  size_t first = 0, last = nmemb - 1;

  while (first <= last) {
    size_t middle = (first + last)/2;
    int cmp = compar(key, &array[middle]);
    if (!cmp) return &array[middle];
    if (cmp > 0) first = middle + 1;
    else last = middle - 1;
  }
  return first > last ? NULL : &array[first];
}

void *lfind(const void *key, const void *base, size_t *nmemb, size_t size,
    int (*compar)(const void *, const void *)) {

  typedef char type[size];
  type *array = (void *)base;
  for (size_t i = 0; i < *nmemb; i++) if (!compar(key, &array[i])) return &array[i];
  return NULL;
}

void *lsearch(const void *key, void *base, size_t *nmemb, size_t size,
    int (*compar)(const void *, const void *)) {

  typedef char type[size];
  type *array = base;
  size_t i = 0;
  for ( ; i < *nmemb; i++) if (!compar(key, &array[i])) return &array[i];
  memcpy(&array[i], key, size);
  return &array[i];
}


