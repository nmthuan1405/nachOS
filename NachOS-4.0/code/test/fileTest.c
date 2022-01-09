#include "syscall.h"

int main(){
    int result;
    int fd, type;
    int len;
    char buffer[100];
    char* text = "Hello World!\n";
    
    // test syscall Open
    PrintString("Open read only testFile1.txt\n");
    fd = Open("./test/testFile1.txt", 1);

    PrintString("Read from testFile: ");
    Read(buffer, 10, fd);
    PrintString(buffer); 

    Close(fd);
    PrintString("Closed testFile1.txt\n");

    // test syscall CreateFile
    PrintString("Create testFile2.txt\n");
    result = CreateFile("./test/testFile2.txt");
    if (result == 0)
        PrintString("Create file successfully.\n");
    else
        PrintString("Create file unsuccessfully.\n");

    PrintString("Open read write testFile2.txt\n");
    fd = Open("./test/testFile2.txt", 0);

    // test read write console
    PrintString("Enter a text to write to opened file: ");
    len = Read(buffer, 10, 0);
    PrintString("The entered text is: ");
    Write(buffer, len, 1);

    // write the entered text to file
    Write(buffer, len, fd);

    Close(fd);
    PrintString("\nClosed testFile2.txt\n");

    Halt();
    return 0;
}