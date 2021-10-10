#include "syscall.h"
#define MAX_LENGTH 10
int main()
{
    char a[MAX_LENGTH];
    PrintString("Enter a string: ");
    ReadString(a, MAX_LENGTH);

    PrintString("You have entered: ");
    PrintString(a);
    PrintChar('\n');

    Halt();
    /* not reached */
}