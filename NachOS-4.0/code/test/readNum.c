#include "syscall.h"

int main()
{
    char result;
    result = ReadChar();
    PrintChar(result);
    Halt();
}
