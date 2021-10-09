#include "syscall.h"

int main()
{
    char result;
    result = ReadChar();
    Halt();
}
