#include "syscall.h"
#define INPUT_FILE "./test/input.txt"
#define OUTPUT_FILE "./test/output.txt"
#define STUDENT_PROG "./test/student"

typedef OpenFileId;
typedef SpaceId;

void createWaterSempahore(){
    if (CreateSemaphore("water", 1) == -1){
        PrintString("Create semaphore error\n");
        Exit(1);
    }
}

void createOutputFile(char* fileName) {
    if (CreateFile(fileName) == -1){
        PrintString("Create file error\n");
        Exit(1);
    }
}

int readInputNumber(char* fileName) {
    OpenFileId inputId;
    int result;

    inputId = Open(fileName, 1);
    if (inputId == -1){
        PrintString("Open file error\n");
        Exit(1);
    }

    if (Read(&result, 1, inputId) == -1){
        PrintString("Read file error\n");
        Exit(1);
    }

    return result - '0';
}

int main(){
    SpaceId id[10];
    int n, i;

    // Tao semaphore cho voi nuoc
    createWaterSempahore();

    // Tao file de ghi ket qua
    createOutputFile(OUTPUT_FILE);

    // Doc so luong nguoi
    n = readInputNumber(INPUT_FILE);

    // Xu ly
    for (i = 0; i < n; i++){
        id[i] = Exec(STUDENT_PROG);
    }

    // Join
    for (i = 0; i < n; i++){
        Join(id[i]);
    }

    return 0;
}