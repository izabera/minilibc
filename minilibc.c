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

#include "my_string.h.c"
#include "my_stdio.h.c"

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

int main(int argc, char *argv[]) {
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
  while (--argc) {
    my_puts(*++argv);
    my_remove(*argv);
  }
  return 0;
}
