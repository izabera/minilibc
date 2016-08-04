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
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

/*#include <linux/memfd.h>*/
#define MFD_CLOEXEC   0x0001U
#define MFD_ALLOW_SEALING 0x0002U

#include "my_stdio.h.c"
#include "my_constructor.c"
#include "my_string.h.c"
#include "my_ctype.h.c"
#include "my_random.h.c"
#include "my_destructor.c"

int main() {
  return 0;
}
