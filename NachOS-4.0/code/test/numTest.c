#include "syscall.h"
int main()
{
    int a;
    PrintString("Enter a number: ");
    a = ReadNum();

    PrintString("You have entered: ");
    PrintNum(a);
    PrintChar('\n');

    PrintString("Random number: ");
    PrintNum(RandomNum());
    PrintChar('\n');

    Halt();
    /* not reached */
}