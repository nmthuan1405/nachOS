#include "syscall.h"

int main()
{
    PrintNum(0);
    PrintString("\n");
    PrintNum(-12345);
    PrintString("\n");
    PrintNum(67890);
    PrintString("\n");
    PrintString("Hello World!\n");
    PrintString("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
    Halt();
    /* not reached */
}