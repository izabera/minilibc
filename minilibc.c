#define _GNU_SOURCE
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>

/*#include <linux/memfd.h>*/
#define MFD_CLOEXEC   0x0001U
#define MFD_ALLOW_SEALING 0x0002U

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

size_t strnlen(const char *s, size_t n) {
  size_t i;
  for (i = 0; i < n; i++)
    if (!s[i]) break;
  return i;
}

typedef struct {
  char buf[BUFSIZ];
  char **ptr;
  size_t *size, used, pos;
  mode_t mode;
  char bufmode, // 'l' line 'f' fully 'n' no
       error,   // 0 fine 'e' eof 'u' unknown
       unget[2]; // [0] == is there a char?    [1] == actual char
  int fd;
} file;

file _my_stdin  = { .fd = 0, .bufmode = 'f' },
     _my_stdout = { .fd = 1, .bufmode = 'f' },
     _my_stderr = { .fd = 2, .bufmode = 'f' },
     *my_stdout = &_my_stdout,
     *my_stdin  = &_my_stdin ,
     *my_stderr = &_my_stderr;
/*#define EOF -1*/

file *my_open_memstream(char **ptr, size_t *size) {
  file *f = malloc(sizeof(*f)); // malloc never fails!(tm)
  f->bufmode = 'f';
  f->ptr = ptr;
  f->size = size;
  f->size[0] = f->used = 0;
  f->fd = syscall(__NR_memfd_create, "tmp", 0); // linux-specific dark magic
  return f;
}

int my_fflush(file *f) {
  f->unget[0] = 0;
  if (f->used) {
    int ret = write(f->fd, f->buf, f->used);
    if (f->size) f->size[0] += ret;
  }
  f->used = 0;
  return 0;
}

int my_feof(file *f) { return f->error == 'e'; }
int my_ferror(file *f) { return !!f->error; }
void my_clearerr(file *f) { f->error = 0; }

int my_fputc(int c, file *f) {
  f->buf[f->used++] = c;
  if (f->bufmode == 'n' || (c == '\n' && f->bufmode == 'l') || f->used == sizeof(f->buf)) my_fflush(f);
  return my_feof(f) ? -1 : c;
}
int my_putc(int c, file *f) __attribute__((weak, alias("my_fputc")));

int my_putchar(int c) { return my_fputc(c, my_stdout); }

int my_fputs(const char *s, file *f) {
  while (*s) my_fputc(*s++, f);
  return 0;
}

int my_fclose(file *f) {
  my_fflush(f);
  if (f->ptr) f->ptr[0] = mmap(NULL, f->size[0], PROT_READ|PROT_WRITE, MAP_PRIVATE, f->fd, 0);
  close(f->fd);
  free(f);
  return 0;
}

int my_fileno(file *f) { return f->fd; }

mode_t strtomode(const char *mode) {
  if (!my_strcmp("r" , mode)) return O_RDONLY;
  if (!my_strcmp("r+", mode)) return O_RDWR;
  if (!my_strcmp("w" , mode)) return O_CREAT | O_TRUNC | O_WRONLY;
  if (!my_strcmp("w+", mode)) return O_CREAT | O_TRUNC | O_RDWR;
  if (!my_strcmp("a" , mode)) return O_CREAT | O_TRUNC | O_APPEND;
  if (!my_strcmp("a+", mode)) return O_CREAT | O_TRUNC | O_APPEND | O_RDWR;
  return -1;
}

file *my_fopen(const char *path, const char *mode) {
  mode_t flags = strtomode(mode);
  int fd = open(path, flags, 0666);
  if (fd == -1) return NULL;
  file *f = malloc(sizeof(*f));
  f->bufmode = isatty(fd) ? 'l' : 'f';
  f->fd = fd;
  f->mode = flags;
  return f;
}

file *my_fdopen(int fd, const char *mode) {
  (void) mode;
  file *f = malloc(sizeof(*f));
  f->bufmode = isatty(fd) ? 'l' : 'f';
  f->fd = fd;
  return f;
}

file *my_freopen(const char *path, const char *mode, file *f) {
  mode_t flags = strtomode(mode);
  int fd = open(path, flags, 0666);
  if (fd == -1) return NULL;
  close(f->fd);
  f->bufmode = isatty(fd) ? 'l' : 'f';
  f->fd = fd;
  f->mode = flags;
  return f;
}

int my_puts(const char *s) {
  my_fputs(s, my_stdout);
  my_fputc('\n', my_stdout);
  return 0;
}

int my_fgetc(file *f) {
  if (f->unget[0]) {
    f->unget[0] = 0;
    return f->unget[1];
  }
  if (f->used > f->pos) return f->buf[f->pos++];
  ssize_t ret = read(f->fd, f->buf, sizeof(f->buf));
  switch (ret) {
    case -1: f->error = 'u'; return -1;
    case  0: f->error = 'e'; return -1;
    default: f->used = ret;
             f->pos = 1;
             return f->buf[0];
  }
}

int my_getc(int c, file *f) __attribute__((weak, alias("my_fgetc")));

int my_getchar(void) { return my_fgetc(my_stdin); }

size_t my_fread(void *ptr, size_t size, size_t nmemb, file *f) {
  if (nmemb == 0) return 0;
  // holy inefficiency
  size_t siz, memb = nmemb;
  int c;
  char *str = ptr;
  do {
    siz = size;
    while (siz--) {
      if ((c = my_fgetc(f)) == EOF) goto ret;
      *str++ = c;
    }
  } while (--memb);
ret: return nmemb - memb;
}

size_t my_fwrite(void *ptr, size_t size, size_t nmemb, file *f) {
  if (nmemb == 0) return 0;
  size_t siz, memb = nmemb;
  char *str = ptr;
  do {
    siz = size;
    while (siz--) if (my_fputc(*str++, f) == EOF) goto ret;
  } while (--memb);
ret: return nmemb - memb;
}

int my_fseek(file *f, long offset, int whence) {
  my_fflush(f); // std c requires the *user* to fflush??
  f->used = 0;
  off_t ret = lseek(f->fd, offset, whence);
  if (f->size) f->size += ret;
  return ret;
}

void my_rewind(file *f) { 
  my_fseek(f, 0, SEEK_SET);
}

int my_ungetc(int c, file *f) {
  if (c == EOF) return EOF;
  f->unget[0] = 1;
  return f->unget[1] = c;
}

char *my_fgets(char *s, int size, file *f) {
  char *ret = s;
  int c;
  for (size_t i = 0; i < (size_t) size - 1; i++) {
    if ((c = my_fgetc(f)) == EOF || c == '\n') break;
    *s++ = c;
  }
  if (s == ret) return NULL;
  *s = 0;
  return ret;
}

long my_ftell(file *f) {
  my_fflush(f);
  return lseek(f->fd, 0, SEEK_CUR);
}

// musl does this
void my_setbuf(file *f, char *buf) { (void) f; (void) buf; }
void my_setbuffer(file *f, char *buf, size_t size) { (void) f; (void) size; (void) buf; }
void my_setlinebuf(file *f) { f->bufmode = 'l'; }

int my_setvbuf(file *f, char *buf, int mode, size_t size) {
  (void) buf;
  (void) size;
  if (mode == _IONBF) f->bufmode = 'n';
  else if (mode == _IOLBF) f->bufmode = 'l';
  else return -1;
  return 0;
}

struct timespec ts;
__attribute__((constructor)) void my_setupstreams() {
  if (isatty(0)) _my_stdin .bufmode = 'n';
  if (isatty(1)) _my_stdout.bufmode = 'l';
  if (isatty(2)) _my_stderr.bufmode = 'n';
  clock_gettime(CLOCK_REALTIME, &ts);
  srand(ts.tv_nsec);
}

__attribute__((destructor)) void my_clearstreams() {
  if (isatty(my_stdout->fd)) my_fflush(my_stdout);
  if (isatty(my_stderr->fd)) my_fflush(my_stderr);
}

int my_mkostemps(char *template, int len, int flags) {
  size_t suff = strlen(template) - len - 6;
  if (strncmp(template+suff, "XXXXXX", 6)) return -1;
  int fd;
  flags -= flags & O_ACCMODE;
  for (int q = 0; q < 100; q++) {
    for (int i = 0; i < 6; i++)
      template[suff+i] = "qwertyuiopasdfghjklzxcvbnm"
                         "QWERTYUIOPASDFGHJKLZXCVBNM" "1234567890" [rand() % 62];
    if ((fd = open(template, flags | O_RDWR | O_CREAT | O_EXCL, 0600)) != -1) return fd;
    if (errno != EEXIST) break;
  }
  return -1;
}

int my_mkstemp(char *template) { return my_mkostemps(template, 0, 0); }
int my_mkstemps(char *template, int len) { return my_mkostemps(template, len, 0); }
int my_mkostemp(char *template, int flags) { return my_mkostemps(template, 0, flags); }

char *my_mktemp(char *template) {
  int fd;
  if ((fd = my_mkstemp(template)) == -1) return NULL;
  close(fd);
  unlink(template);
  return template;
}

char *my_tempnam(const char *dir, const char *pfx) {
  char *template = malloc(PATH_MAX);
  snprintf(template, PATH_MAX, "%s/%sXXXXXX", dir, pfx);
  return my_mktemp(template);
}

file *my_tmpfile(void) {
  file *f = calloc(1, sizeof(*f));
  f->fd = syscall(__NR_memfd_create, "tmp", 0);
  f->bufmode = 'f';
  f->mode = strtomode("r+");
  return f;
}

char *my_tmpnam(char *s) {
  static char buf[PATH_MAX] = "/tmp/XXXXXX";
  char *template = s;
  if (s) my_memmove(s, buf, strlen(buf)+1);
  else template = buf;
  int fd;
  if ((fd = my_mkstemp(template)) == -1) return NULL;
  close(fd);
  return template;
}

int main() {
  /*int c = my_getchar();*/
  /*my_putchar('<');*/
  /*my_putchar(c);*/
  /*my_putchar('>');*/
  /*char buf[20];*/
  /*my_fwrite(buf, 1, my_fread(buf, 1, sizeof(buf), my_stdin), my_stdout);*/
  /*char *ptr;*/
  /*size_t size;*/
  /*file *f = my_open_memstream(&ptr, &size);*/
  /*for (int i = 0; i < 100; i++) my_fputs("hello world! ", f);*/
  /*my_fclose(f);*/
  /*my_puts("helloooo");*/
  /*my_puts(ptr);*/
  /*char buf[20];*/
  /*my_puts(my_fgets(buf, sizeof(buf), my_stdin));*/
  /*my_puts(my_tempnam("/tmp", "lol"));*/
  /*file *f = my_tmpfile();*/
  /*my_fputs("hello\n", f);*/
  /*my_fflush(f);*/
  /*char path[PATH_MAX];*/
  /*snprintf(path, PATH_MAX, "/proc/self/fd/%d", f->fd);*/
  /*linkat(AT_FDCWD, path, AT_FDCWD, "/tmp/file", AT_SYMLINK_FOLLOW);*/
  /*my_puts(my_tmpnam(path));*/
  return 0;
}
