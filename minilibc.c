#define _GNU_SOURCE
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/memfd.h>

typedef struct {
  char buf[4096];
  char **ptr;
  size_t *size, used;
  int fd;
} file;

file *my_open_memstream(char **ptr, size_t *size) {
  file *f = malloc(sizeof(*f));
  f->ptr = ptr;
  f->size = size;
  f->size[0] = f->used = 0;
  f->fd = syscall(__NR_memfd_create, "tmp", 0);
  return f;
}

int my_fflush(file *f) {
  if (f->used) {
    int ret = write(f->fd, f->buf, f->used);
    if (f->size) f->size[0] += ret;
  }
  f->used = 0;
  return 0;
}

int my_fputc(int c, file *f) {
  f->buf[f->used++] = c;
  if (f->used == sizeof(f->buf)) my_fflush(f);
  return (unsigned char) c;
}

int my_fputs(const char *s, file *f) {
  while (*s) my_fputc(*s++, f);
  return 0;
}

int my_fclose(file *f) {
  my_fflush(f);
  f->ptr[0] = mmap(NULL, f->size[0], PROT_READ|PROT_WRITE, MAP_PRIVATE, f->fd, 0);
  close(f->fd);
  free(f);
  return 0;
}

mode_t strtomode(const char *mode) {
  if (strcmp("r" , mode)) return O_RDONLY;
  if (strcmp("r+", mode)) return O_RDWR;
  if (strcmp("w" , mode)) return O_CREAT | O_TRUNC | O_WRONLY;
  if (strcmp("w+", mode)) return O_CREAT | O_TRUNC | O_RDWR;
  if (strcmp("a" , mode)) return O_CREAT | O_TRUNC | O_APPEND;
  if (strcmp("a+", mode)) return O_CREAT | O_TRUNC | O_APPEND | O_RDWR;
  return -1;
}

file *my_fopen(const char *path, const char *mode) {
  file *f = malloc(sizeof(*f));
  f->fd = open(path, strtomode(mode));
  return f;
}

file *my_fdopen(int fd, const char *mode) {
  file *f = malloc(sizeof(*f));
  f->fd = fd;
  return f;
}

file *my_freopen(const char *path, const char *mode, file *stream) {
  close(stream->fd);
  stream->fd = open(path, strtomode(mode));
  return stream;
}

file _my_stdin  = { .fd = 0 },
     _my_stdout = { .fd = 1 },
     _my_stderr = { .fd = 2 },
     *my_stdout = &_my_stdout,
     *my_stdin  = &_my_stdin ,
     *my_stderr = &_my_stderr;

int my_puts(const char *s) {
  return my_fputs(s, my_stdout) + my_fputc('\n', my_stdout);
}

int main() {
  char *ptr;
  size_t size;
  file *f = my_open_memstream(&ptr, &size);
  for (int i = 0; i < 1000; i++) my_fputs("hello world! ", f);
  my_fclose(f);
  my_puts("helloooo");
  my_puts(ptr);
  my_fflush(my_stdout);
  return 0;
}
