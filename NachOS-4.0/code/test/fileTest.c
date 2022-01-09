#include "syscall.h"

int main(){
    int result;
    int fd, type;
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
    Write(text, 20, fd);
    //PrintString("Read from testFile: ");
    Read(buffer, 20, fd);
    PrintString(buffer); // print ko ra 

    // test read write console
    PrintString("\nEnter a text to write to opened file: ");
    Read(buffer, 10, 0);
    PrintString("The entered text is: ");
    Write(buffer, 10, 1);

    // write the entered text to file
    Write(buffer, 10, fd);

    // close file
    Close(fd);

    Halt();
    return 0;
}