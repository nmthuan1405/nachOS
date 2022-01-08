#include "syscall.h"

int main(){
    int result;
    int fd;
    char buffer[100];
    char* text = "Hello World!\n";
    
    // result = CreateFile("test.txt");

    fd = Open("a.txt", 0);
    Read(buffer, 100, fd);
    Write(text, 5, fd);
    PrintString(buffer);

    Halt();
    return 0;
}