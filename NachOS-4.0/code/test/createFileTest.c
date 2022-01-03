#include "syscall.h"

int main(){
    int result;
    
    result = CreateFile("test.txt");

    Halt();
    return 0;
}