#include "random.h"
#include "prototypes.h"

#include <string.h>

void *memccpy(void *dest, const void *src, int c, size_t n) {
  char *p1 = dest;
  const char *p2 = src;
  while (n--) if ((*p1++ = *p2++) == c) return p1;
  return NULL;
}

void *memchr(const void *s, int c, size_t n) {
  const char *p = s;
  while (n--) {
    if (*p == c) return (char *)p;
    p++;
  }
  return NULL;
}

void *memrchr(const void *s, int c, size_t n) {
  const char *p = s;
  while (n) {
    if (p[n] == c) return (char *)p+n;
    n--;
  }
  return NULL;
}

void *rawmemchr(const void *s, int c) {
  const char *p = s;
  while (*p != c) p++;
  return (void *)p;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const char *p1 = s1, *p2 = s2;
  while (*p1 == *p2 && n--) p1++, p2++;
  return n ? *p1 - *p2 : 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
  char *p1 = dest; const char *p2 = src;
  while (n--) *p1++ = *p2++;
  return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
  char *p1 = dest; const char *p2 = src;
  size_t m = n;
  if (p1 < p2) while (n) p1[n--] = p2[m--];
  else while (n--) *p1++ = *p2++;
  return dest;
}

void *memset(void *s, int c, size_t n) {
  char *p = s;
  while (n--) *p++ = c;
  return s;
}

char *stpncpy(char *dest, const char *src, size_t n) {
  while (n-- && (*dest++ = *src++));
  return dest;
}

char *stpcpy(char *dest, const char *src) {
  while ((*dest++ = *src++));
  return dest;
}

char *strcpy(char *dest, const char *src) {
  char *ret = dest;
  do { *dest++ = *src; } while (*src++);
  return ret;
}

char *strncpy(char *dest, const char *src, size_t n) {
  const char *ret = dest;
  do { *dest++ = *src; } while (n-- && *src++);
  return (char *)ret;
}

size_t strlen(const char *str) {
  size_t l = 0;
  while (*str++) l++;
  return l;
}

char *strcat(char *dest, const char *src) {
  char *ret = dest;
  dest += strlen(dest);
  do { *dest++ = *src; } while (*src++);
  return ret;
}

char *strncat(char *dest, const char *src, size_t n) {
  char *ret = dest;
  dest += strlen(dest);
  do { *dest++ = *src; } while (n-- && *src++);
  *dest = 0;
  return ret;
}

char *strchr(const char *str, int c) {
  while (*str) {
    if (*str == c) return (char *)str;
    str++;
  }
  return NULL;
}

char *strrchr(const char *str, int c) {
  const char *begin = str;
  str += strlen(str);
  while (str != begin) {
    if (*str == c) return (char *)str;
    str--;
  }
  return NULL;
}

char *strchrnul(const char *str, int c) {
  while (*str) {
    if (*str == c) return (char *)str;
    str++;
  }
  return (char *)str;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && *s1 == *s2) s1++, s2++;
  return *s1 - *s2;
}

int strcasecmp(const char *s1, const char *s2) {
  while (*s1 &&
      (*s1 | (*s1 >= 'A' && *s1 <= 'Z' ? 32 : 0)) ==
      (*s2 | (*s2 >= 'A' && *s2 <= 'Z' ? 32 : 0))      ) s1++, s2++;
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (*s1 && *s1 == *s2 && n--) s1++, s2++;
  return n ? *s1 - *s2 : 0;
}

int strncasecmp(const char *s1, const char *s2, size_t n) {
  while (*s1 &&
      (*s1 | (*s1 >= 'A' && *s1 <= 'Z' ? 32 : 0)) ==
      (*s2 | (*s2 >= 'A' && *s2 <= 'Z' ? 32 : 0))       && n--) s1++, s2++;
  return n ? *s1 - *s2 : 0;
}

size_t strnlen(const char *s, size_t n) {
  size_t i;
  for (i = 0; i < n; i++)
    if (!s[i]) break;
  return i;
}

char *strdup(const char *s) {
  size_t len = strlen(s)+1;
  char *ret = malloc(len);
  return memcpy(ret, s, len);
}

char *strndup(const char *s, size_t n) {
  size_t len = strlen(s)+1;
  len = len > n ? len : n;
  char *ret = malloc(len);
  ret[len-1] = 0;
  return memcpy(ret, s, len-1);
}

char *strstr(const char *h, const char *n) {
  if (!n[0]) return (char *)h;
  h = strchr(h, *n);
  if (!h || !n[1]) return (char *)h;

  size_t nl = 0, same = 1, i; // compute strlen(n) but not strlen(h)
  for (; h[nl] && n[nl]; nl++) if (h[nl] != n[nl]) same = 0;
  if (!h[nl] && !n[nl]) return same ? (char *)h : NULL;
  if (!h[nl]) return NULL;

  for (; *h; h++) {
    for (i = 0; i < nl && h[i] == n[i]; i++);
    if (i == nl) return (char *)h;
  }

  return NULL;
}

char *strcasestr(const char *h, const char *n) {
  if (!n[0]) return (char *)h;
  h = strpbrk(h, (char*) { tolower(*n), toupper(*n) });
  if (!h || !n[1]) return h;

  size_t nl = 0, same = 1, i;
  for (; h[nl] && n[nl]; nl++) if (tolower(h[nl]) != tolower(n[nl])) same = 0;
  if (!h[nl] && !n[nl]) return same ? (char *)h : NULL;
  if (!h[nl]) return NULL;

  for (; *h; h++) {
    for (i = 0; i < nl && tolower(h[i]) == tolower(n[i]); i++);
    if (i == nl) return (char *)h;
  }

  return NULL;
}

#define checkbit(mask, byte) (mask[(unsigned char)byte/8] &  1 << (unsigned char)byte % 8)
#define   setbit(mask, byte) (mask[(unsigned char)byte/8] |= 1 << (unsigned char)byte % 8)
size_t strspn(const char *s, const char *accept) {
  size_t i, slen = strlen(s), acceptlen = strlen(accept);
  if (slen == 0 || acceptlen == 0) return 0;

  char mask[32] = { 0 };
  for (i = 0; i < acceptlen; i++) setbit(mask, accept[i]);
  for (i = 0; i < slen && checkbit(mask, s[i]); i++);

  return i;
}

size_t strcspn(const char *s, const char *accept) {
  size_t i, slen = strlen(s), acceptlen = strlen(accept);
  if (slen == 0 || acceptlen == 0) return slen;

  char mask[32] = { 0 };
  for (i = 0; i < acceptlen; i++) setbit(mask, accept[i]);
  for (i = 0; i < slen && !checkbit(mask, s[i]); i++);

  return i;
}
#undef checkbit
#undef setbit

char *strpbrk(const char *s, const char *accept) {
  s += strcspn(s, accept);
  return *s ? (char *)s : NULL;
}

void *memfrob(void *s, size_t n) {
  for (char *p = s; n; n--) *p++ ^= 42;
  return s;
}

char *strfry(char *s) { // glibc uses the naive swap??? BOOOOOOOOO
  size_t len = strlen(s), rnum;
  for (size_t i = len - 1; i > 0; i--) {
    rnum = pcg32_random_r(&__randnum) % (i + 1);
    char c = s[rnum];
    s[rnum] = s[i];
    s[i] = c;
  }
  return s;
}

size_t strlcat(char *dest, const char *src, size_t size) {
  size_t ret;
  for (ret = 0; ret < size && dest[ret]; ret++);
  if (ret == size) return ret + strlen(src);
  for (; ret < size-1 && *src; ret++) dest[ret] = *src++;
  dest[ret] = 0;
  return ret + strlen(src);
}

size_t strlcpy(char *dest, const char *src, size_t size) {
  size_t ret;
  for (ret = 0; ret < size-1 && *src; ret++) dest[ret] = *src++;
  dest[ret] = 0;
  return ret;
}

int strverscmp(const char *s1, const char *s2) {
  while (*s1 && *s1 == *s2) s1++, s2++;
  if (!*s1 && !*s2) return 0;
  if (!*s1) return -1;
  if (!*s2) return 1;
  if (isdigit(*s1) && isdigit(*s2)) return strtoul(s1, NULL, 10) - strtoul(s2, NULL, 10);
  return strcmp(s1, s2);
}
