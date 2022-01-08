#include "syscall.h"

int main(){

    // Tao semaphore cho voi nuoc
    int isCreateSem = CreateSemaphore("water", 1);
    if (isCreateSem == -1){
        return 1;
    }

    // Mo file chi doc input.txt
    OpenFileId inputId = Open("input.txt", 1);
    if (inputId == -1){
        return 1;
    }

    // Doc so luong sinh vien tu file input.txt
    char buffer;
    int isRead = Read(&buffer, 1, inputId);
    if (isRead == -1){
        return 1;
    }
    int n = buffer - '0';
    
    // Xu ly
    SpaceId id[5];
    for (int i = 0; i < n; i++){
        id[i] = Exec("./test/student");
    }

    // Join
    for (int i = 0; i < n; i++){
        Join(id[i]);
    }

    return 0;
}