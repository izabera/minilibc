__attribute__((destructor)) void my_cleanup() {
  if (isatty(my_stdout->fd)) my_fflush(my_stdout);
  if (isatty(my_stderr->fd)) my_fflush(my_stderr);
}
