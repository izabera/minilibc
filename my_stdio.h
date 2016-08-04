#ifndef _MY_STDIO_H
#define _MY_STDIO_H
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
#endif
