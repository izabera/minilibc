#include "my_random.h"
#include "prototypes.h"

struct timespec __ts;
pcg32_random_t __randnum;

__attribute__((constructor)) void my_setup() {
  if (isatty(0)) _my_stdin .bufmode = 'n';
  if (isatty(1)) _my_stdout.bufmode = 'l';
  if (isatty(2)) _my_stderr.bufmode = 'n';
  clock_gettime(CLOCK_REALTIME, &__ts);
  __randnum.state = __ts.tv_nsec;
  __randnum.inc   = __ts.tv_sec;
  pcg32_random_r(&__randnum);
}
