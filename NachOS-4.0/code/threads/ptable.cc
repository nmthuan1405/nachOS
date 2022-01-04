
#include "ptable.h"

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