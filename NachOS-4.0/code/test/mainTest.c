#include "syscall.h"

typedef OpenFileId;
typedef SpaceId;

int main(){
    int isCreateSem, n, i, isRead;
    OpenFileId inputId;
    char buffer;
    SpaceId id[5];

    // Tao semaphore cho voi nuoc
    isCreateSem = CreateSemaphore("water", 1);
    if (isCreateSem == -1){
        return 1;
    }

    // Mo file chi doc input.txt
    inputId = Open("input.txt", 1);
    if (inputId == -1){
        return 1;
    }

    // Doc so luong sinh vien tu file input.txt
    isRead = Read(&buffer, 1, inputId);
    if (isRead == -1){
        return 1;
    }
    n = buffer - '0';
    
    // Xu ly
    for (i = 0; i < n; i++){
        id[i] = Exec("./test/student");
    }

    // Join
    for (i = 0; i < n; i++){
        Join(id[i]);
    }

    return 0;
}