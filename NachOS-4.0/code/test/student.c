#include "syscall.h"

typedef OpenFileId;

void getWater(){
    int i;
    for (i = 0; i < 1000; i++){

    }
    PrintNum(GetCurrentProcessId());
    PrintString("Lay nuoc\n");
}

int main(){
    int i;
    OpenFileId outputId;
    char id;
    for (i = 0; i < 10; i++){
        // Acquire voi nuoc
        Wait("water");

        // Lay 1 lit nuoc
        getWater();

        // Mo file doc ghi output.txt
        outputId = Open("./test/output.txt", 0);
        if (outputId == -1){
            return 1;
        }

        // Ghi id tien trinh (sinh vien) vao file output.txt
        id = GetCurrentProcessId() + '0';
        Write(&id, 1, outputId);
        Write(" ", 1, outputId);

        // Dong file output.txt
        Close(outputId);

        // Release voi nuoc
        Signal("water");
    }

    return 0;
}