#include "syscall.h"

int
main()
{
  PrintChar('a');

  Halt();
  /* not reached */
}