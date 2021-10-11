#include "syscall.h"

int main()
{
    int i;

    PrintString("--------------- ASCII TABLE ---------------\n");
    for (i = 32; i <= 127; ++i)
    {
        if (i < 100)
            PrintChar(' ');
        PrintNum(i);
        PrintString(": ");
        PrintChar(i);

        if ((i - 31) % 4 == 0)
            PrintChar('\n');
        else
            PrintString("      ");
    }

    Halt();
}
