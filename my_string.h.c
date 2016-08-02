void *my_memccpy(void *dest, const void *src, int c, size_t n) {
  char *p1 = dest;
  const char *p2 = src;
  while (n--) if ((*p1++ = *p2++) == c) return p1;
  return NULL;
}

void *my_memchr(const void *s, int c, size_t n) {
  const char *p = s;
  while (n--) {
    if (*p == c) return (char *)p;
    p++;
  }
  return NULL;
}

void *my_memrchr(const void *s, int c, size_t n) {
  const char *p = s;
  while (n) {
    if (p[n] == c) return (char *)p+n;
    n--;
  }
  return NULL;
}

void *my_rawmemchr(const void *s, int c) {
  const char *p = s;
  while (*p != c) p++;
  return (void *)p;
}

int my_memcmp(const void *s1, const void *s2, size_t n) {
  const char *p1 = s1, *p2 = s2;
  while (*p1 == *p2 && n--) p1++, p2++;
  return *p1 - *p2;
}

void *my_memcpy(void *dest, const void *src, size_t n) {
  char *p1 = dest; const char *p2 = src;
  while (n--) *p1++ = *p2++;
  return dest;
}

void *my_memmove(void *dest, const void *src, size_t n) {
  char *p1 = dest; const char *p2 = src;
  size_t m = n;
  if (p1 < p2) while (n) p1[n--] = p2[m--];
  else while (n--) *p1++ = *p2++;
  return dest;
}

void *my_memset(void *s, int c, size_t n) {
  char *p = s;
  while (n--) *p++ = c;
  return s;
}

char *my_stpcpy(char *dest, const char *src, size_t n) {
  while (n-- && (*dest++ = *src++));
  return dest;
}

char *my_stpcpy(char *dest, const char *src) {
  while ((*dest++ = *src++));
  return dest;
}

char *my_strcpy(char *dest, const char *src) {
  char *ret = dest;
  do { *dest++ = *src; } while (*src++);
  return ret;
}

char *my_strncpy(char *dest, const char *src, size_t n) {
  const char *ret = dest;
  do { *dest++ = *src; } while (n-- && *src++);
  return (char *)ret;
}

size_t my_strlen(const char *str) {
  size_t l = 0;
  while (*str++) l++;
  return l;
}

char *my_strcat(char *dest, const char *src) {
  char *ret = dest;
  dest += my_strlen(dest);
  do { *dest++ = *src; } while (*src++);
  return ret;
}

char *my_strncat(char *dest, const char *src, size_t n) {
  char *ret = dest;
  dest += my_strlen(dest);
  do { *dest++ = *src; } while (n-- && *src++);
  *dest = 0;
  return ret;
}

char *my_strchr(const char *str, int c) {
  while (*str) {
    if (*str == c) return (char *)str;
    str++;
  }
  return NULL;
}

char *my_strrchr(const char *str, int c) {
  const char *begin = str;
  str += my_strlen(str);
  while (str != begin) {
    if (*str == c) return (char *)str;
    str--;
  }
  return NULL;
}

char *my_strchrnul(const char *str, int c) {
  while (*str) {
    if (*str == c) return (char *)str;
    str++;
  }
  return (char *)str;
}

int my_strcmp(const char *s1, const char *s2) {
  while (*s1 && *s1 == *s2) s1++, s2++;
  return *s1 - *s2;
}

int my_strncmp(const char *s1, const char *s2, size_t n) {
  while (*s1 && *s1 == *s2 && n--) s1++, s2++;
  return *s1 - *s2;
}

size_t my_strnlen(const char *s, size_t n) {
  size_t i;
  for (i = 0; i < n; i++)
    if (!s[i]) break;
  return i;
}

char *my_strdup(const char *s) {
  size_t len = strlen(s)+1;
  char *ret = malloc(len);
  return my_memcpy(ret, s, len);
}

char *my_strndup(const char *s, size_t n) {
  size_t len = strlen(s)+1;
  len = len > n ? len : n;
  char *ret = malloc(len);
  ret[len-1] = 0;
  return my_memcpy(ret, s, len-1);
}