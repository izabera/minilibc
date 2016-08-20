#include "prototypes.h"

long int my_strtol(const char *s, char **end, int base) {
  unsigned long int ret = my_strtoumax(s, end, base);
  if (ret > LONG_MAX) return ret - LONG_MAX - 1;
  return ret;
}

long long int my_strtoll(const char *s, char **end, int base) {
  unsigned long long int ret = my_strtoumax(s, end, base);
  if (ret > LLONG_MAX) return ret - LLONG_MAX - 1;
  return ret;
}

int  my_atoi(const char *s) { return my_strtol(s, NULL, 10); }
long my_atol(const char *s) { return my_strtol(s, NULL, 10); }
long long my_atoll(const char *s) { return my_strtoll(s, NULL, 10); }

// double my_atof(const char *s) { return my_strtod(s, NULL); } */

unsigned long int my_strtoul(const char *s, char **end, int base) {
  return my_strtoumax(s, end, base);
}

unsigned long long int my_strtoull(const char *s, char **end, int base) {
  return my_strtoumax(s, end, base);
}

uintmax_t my_strtoumax(const char *s, char **end, int base) {
  if ((base < 2 && base != 0) || base > 36) {
    if (end) *end = (char *)s;
    return 0;
  }
  while (my_isspace(*s)) s++;
  uintmax_t ret = 0, neg = 1;
  switch (*s) {
    case '-': neg = -1;
    case '+': s++;
  }
  switch (base) {
    case 0: if (s[0] == '0' && (s[1]|32) == 'x') {
              base = 16;
              s += 2;
            }
            else if (s[0] == '0') {
              base = 8;
              s++;
            }
            else base = 10;
            break;
    case 16: if (s[0] == '0' && (s[1]|32) == 'x') s += 2;
  }
  while (*s == '0') s++;

#define __isvalid(x) ((base <= 10 && x >= '0' && x < '0' + base) || \
                      ((x >= '0' && x <= '9')       ||              \
                       (x >= 'A' && x < 'A' + base) ||              \
                       (x >= 'a' && x < 'a' + base)))
#define __value(c) ((c) >= 'A' ? ((c)|32) - 'a' + 10 : (c) - '0')
  for (char c = *s; __isvalid(c); c = *++s)
    ret = ret * base + __value(c);
#undef __value
#undef __isvalid

  if (end) *end = (char *)s;
  return neg * ret;
}

#define min(a,b) ((a) < (b) ? (a) : (b))
static void swap(void *a, void *b, size_t size) {
  for (char tmp[256]; size; size -= min(size, sizeof(tmp))) {
    memcpy(tmp, a, min(size, sizeof(tmp)));
    memcpy(a,   b, min(size, sizeof(tmp)));
    memcpy(b, tmp, min(size, sizeof(tmp)));
  }
}

static void my_actual_sort(void *base, size_t nmemb, size_t size,
    int (*f1)(const void *, const void *),
    int (*f2)(const void *, const void *, void *),
    void *arg) {

  char *ptr1 = base, *ptr2;

#define cmp(a,b) (f1 ? f1(a,b) : f2(a,b,arg))
// optimize a few cases
  if (nmemb < 2) return;
  if (nmemb == 2) {
    ptr2 = (char *)base + size;
    if (cmp(base, ptr2, arg) < 0) swap(ptr1, ptr2, size);
    return;
  }
  if (nmemb < 10) {
    for (; ptr1 < (char *)base + nmemb * size; ptr1 += size) {
      for (ptr2 = ptr1; ptr2 > (char *)base && cmp(ptr2, ptr2 - size, arg) < 0; ptr2 -= size)
        swap(ptr2, ptr2 - size, size);
    }
    return;
  }

  // heapify
  for (size_t i = (nmemb - 2) / 2; i > 0; i--) {
  }
#undef cmp

}

void my_qsort_r(void *b, size_t n, size_t s, int (*f)(const void *, const void *, void *), void *a) {
  my_actual_sort(b, n, s, NULL, f, a);
}

void my_qsort(void *b, size_t n, size_t s, int (*f)(const void *, const void *)) {
  my_actual_sort(b, n, s, f, NULL, NULL);
}
