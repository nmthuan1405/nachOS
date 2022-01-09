#include "ptable.h"

PTable::PTable(int size)
{
    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem", 1);

    for (int i = 0; i < MAX_PROCESS; i++)
    {
        pcb[i] = NULL;
    }

    bm->Mark(0);
    pcb[0] = new PCB();
    pcb[0]->parentID = -1;
}

PTable::~PTable()
{
    if (bm != NULL)
    {
        delete bm;
        bm = NULL;
    }

    if (bmsem != NULL)
    {
        delete bmsem;
        bmsem = NULL;
    }

    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (pcb[i] != NULL)
        {
            delete pcb[i];
            pcb[i] = NULL;
        }
    }
}

int PTable::ExecUpdate(char *name)
{
    // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc
    bmsem->P();

    // Kiểm tra tính hợp lệ của chương trình “name”
    if (name == NULL || strlen(name) == 0)
    {
        bmsem->V();
        return -1;
    }

    // Kiểm tra sự tồn tại của chương trình “name” bằng cách gọi phương thức Open của lớp fileSystem
    OpenFile *file = kernel->fileSystem->Open(name);
    if (file == NULL)
    {
        bmsem->V();
        return -1;
    }
    delete file;

    // So sánh tên chương trình và tên của currentThread để chắc chắn rằng chương trình này không gọi thực thi chính nó.
    if (strcmp(name, kernel->currentThread->getName()) == 0)
    {
        bmsem->V();
        return -1;
    }

    // Tìm slot trống trong bảng Ptable
    int index = GetFreeSlot();
    if (index < 0)
    {
        bmsem->V();
        return -1;
    }

    // Nếu có slot trống thì khởi tạo một PCB mới với processID chính là index của slot này, parrentID là processID của currentThread.
    pcb[index] = new PCB();
    pcb[index]->parentID = kernel->currentThread->processID;

    // Gọi thực thi phương thức Exec của lớp PCB
    int pid = pcb[index]->Exec(name, index);

    bmsem->V();
    return pid;
}

int PTable::ExitUpdate(int ec)
{
    int id = kernel->currentThread->processID;

    // Neu tien trinh khong ton tai
    if (!IsExist(id))
    {
        printf("The process does not exist\n");
        return -1;
    }

    // Tien trinh la main process
    if (id == 0)
    {
        kernel->interrupt->Halt();
        return 0;
    }

    // Nguoc lai set exitcode cho tien trinh goi
    pcb[id]->SetExitCode(ec);
    pcb[pcb[id]->parentID]->DecNumWait();

    // Goi JoinRelease de giai phong tien trinh cha dang doi
    pcb[id]->JoinRelease();

    // Goi ExitWait de xin tien trinh cha cho phep thoat
    pcb[id]->ExitWait();

    Remove(id);
    return ec;
}

int PTable::JoinUpdate(int id)
{
    // Kiem tra tinh hop le cua id
    if (id < 0 || id >= MAX_PROCESS)
    {
        printf("Invalid ID\n");
        return -1;
    }

    // Kiem tra tien trinh goi join co phai la cha cua tien trinh co processID la id hay khong
    if (kernel->currentThread->processID != pcb[id]->parentID)
    {
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

int PTable::GetFreeSlot()
{
    return bm->FindAndSet();
}

bool PTable::IsExist(int pid)
{
    return bm->Test(pid);
}

void PTable::Remove(int pid)
{
    bm->Clear(pid);
    if (pcb[pid] != NULL)
    {
        delete pcb[pid];
        pcb[pid] = NULL;
    }
}

char *PTable::GetFileName(int id)
{
    pcb[id]->GetFileName();
}

PCB *PTable::getCurrentPCB()
{
    return pcb[kernel->currentThread->processID];
}