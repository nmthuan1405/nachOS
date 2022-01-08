#include "PCB.h"

PCB::PCB()
{
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    multex = new Semaphore("multex", 1);

    numwait = 0;
    exitcode = 0;

    fileTable = new FILE *[MAX_FILE];
    filemap = new Bitmap(MAX_FILE);
    filemap->Mark(0);
    filemap->Mark(1);

    thread = NULL;

    parentID = 0;
}

PCB::~PCB()
{
    if (joinsem != NULL)
    {
        delete joinsem;
        joinsem = NULL;
    }

    if (exitsem != NULL)
    {
        delete exitsem;
        exitsem = NULL;
    }

    if (multex != NULL)
    {
        delete multex;
        multex = NULL;
    }

    if (fileTable != NULL)
    {
        delete[] fileTable;
        fileTable = NULL;
    }

    if (filemap != NULL)
    {
        delete filemap;
        filemap = NULL;
    }

    if (thread != NULL)
    {
        thread->FreeSpace();
        thread->Finish();
        delete thread;
    }
}

int PCB::Exec(char *filename, int id)
{
    // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
    multex->P();

    // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.
    this->thread = new Thread(filename); // (./threads/thread.h)

    if (this->thread == NULL)
    {
        printf("\nPCB::Exec: Not enough memory!\n");
        multex->V(); // Nha CPU de nhuong CPU cho tien trinh khac
        return -1;   // Tra ve -1 neu that bai
    }

    //  Đặt processID của thread này là id.
    this->thread->processID = id;
    // Đặt parrentID của thread này là processID của thread gọi thực thi Exec
    this->parentID = kernel->currentThread->processID;
    // Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
    this->thread->Fork((VoidFunctionPtr)StartProcess, (void *)id);

    multex->V();
    // Trả về id.
    return id;
}

int PCB::GetID()
{
    return thread->processID;
}

int PCB::GetNumWait()
{
    return numwait;
}

void PCB::JoinWait()
{
    joinsem->P();
}

void PCB::ExitWait()
{
    exitsem->P();
}

void PCB::JoinRelease()
{
    joinsem->V();
}

void PCB::ExitRelease()
{
    exitsem->V();
}

void PCB::IncNumWait()
{
    multex->P();
    numwait++;
    multex->V();
}

void PCB::DecNumWait()
{
    multex->P();
    if (numwait > 0)
    {
        numwait--;
    }
    multex->V();
}

void PCB::SetExitCode(int ec)
{
    exitcode = ec;
}

int PCB::GetExitCode()
{
    return exitcode;
}

void PCB::SetFileName(char *fn)
{
    strcpy(thread->name, fn);
}

char *PCB::GetFileName()
{
    return thread->name;
}

void StartProcess(int id)
{
    // Lay fileName cua process id nay
    char *fileName = kernel->pTab->GetFileName(id);

    AddrSpace *space;
    space = new AddrSpace();

    if (space == NULL)
    {
        printf("\nPCB::Exec: Can't create AddSpace.");
        return;
    }

    if (space->Load(fileName))
    {                       // load the program into the space
        space->Execute();   // run the program
        ASSERTNOTREACHED(); // Execute never returns
    }
}