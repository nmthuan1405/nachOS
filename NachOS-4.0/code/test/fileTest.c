#include "syscall.h"

int main(){
    int result;
    int fd, type;
    int len;
    char buffer[100];
    char* text = "Hello World!\n";
    
    // test syscall CreateFile
    PrintString("Create testFile.txt\n");
    result = CreateFile("./test/testFile.txt");
    if (result == 0)
        PrintString("Create file successfully.\n");
    else
        PrintString("Create file unsuccessfully.\n");

    // test syscall Open
    PrintString("Open file...\nEnter type: ");
    //type = ReadNum();
    fd = Open("./test/testFile.txt", 0);

    // write Hello World to testFile
    Write(text, 5, fd);
    PrintString("Read from testFile: ");
    len = Read(buffer, 10, fd);
    buffer[len] = '\0';
    PrintString(buffer); // print ko ra 

    // test read write console
    PrintString("\nEnter a text to write to opened file: ");
    len = Read(buffer, 10, 0);
    PrintString("The entered text is: ");
    Write(buffer, len, 1);

    // write the entered text to file
    Write(buffer, len, fd);

    // close file
    Close(fd);

    Halt();
    return 0;
}