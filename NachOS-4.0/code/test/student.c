#include "syscall.h"

void getWater(){
    for (int i = 0; i < 1000; i++){

    }
}

int main(){

    for (int i = 0; i < 10; i++){
        // Acquire voi nuoc
        Wait("water");

        // Lay 1 lit nuoc
        getWater();

        // Mo file doc ghi output.txt
        OpenFileId outputId = Open("output.txt", 0);
        if (outputId == -1){
            return 1;
        }

        // Ghi id tien trinh (sinh vien) vao file output.txt
        char id = GetCurrentProcessId() + '0';
        Write(id, 1, outputId);
        Wrire(' ', 1, outputId);

        // Dong file output.txt
        Close(outputId);

        // Release voi nuoc
        Signal("water");
    }

    return 0;
}