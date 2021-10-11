#include "syscall.h"

int main()
{
    char a;
    PrintString("Enter a character: ");
    a = ReadChar();

    PrintString("You have entered: ");
    PrintChar(a);
    PrintChar('\n');

    Halt();
    /* not reached */
}