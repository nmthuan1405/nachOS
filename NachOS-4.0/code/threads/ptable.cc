
#include "ptable.h"

PTable::PTable(int size){

    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem", 1);

    for (int i = 0; i < MAX_PROCESS; i++){
        pcb[i] = NULL;
    }

    bm->Mark(0);
}

PTable::~PTable(){
    if (bm != NULL){
        delete bm;
    }

    if (bmsem != NULL){
        delete bmsem;
    }

    for (int i = 0; i < MAX_PROCESS; i++){
        if (pcb[i] != NULL){
            delete pcb[i];
        }
    }
}

int PTable::JoinUpdate(int id){
    // Kiem tra tinh hop le cua id
    if (id < 0 || id > 9){
        printf("Invalid ID\n");
        return -1;
    }

    // Kiem tra tien trinh goi join co phai la cha cua tien trinh co processID la id hay khong
    if (kernel->currentThread->processID != pcb[id]->parentID){
        printf("The process can't join not parent process\n");
        return -1;
    }

    // Tang numWait va goi JoinWait de cho tien trinh con thuc hien
    pcb[pcb[id]->parentID]->IncNumWait();
    pcb[id]->JoinWait();

    // Xu ly exitcode
    int exitcode = pcb[id]->GetExitCode();

    // Goi ExitReleas de cho phep tien trinh con thoat
    pcb[id]->ExitRelease();

    // Tra ve exitcode
    return exitcode;
}