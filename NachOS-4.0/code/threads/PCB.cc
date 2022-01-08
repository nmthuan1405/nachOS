#include "PCB.h"

PCB::PCB()
{
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    multex = new Semaphore("multex", 1);

    numwait = 0;
    exitcode = 0;

    fileTable = new FILE*[MAX_FILE];
    filemap = new Bitmap(MAX_FILE);
    filemap->Mark(0);
    filemap->Mark(1);

    thread = NULL;

    parentID = 0;
}

PCB::PCB(int id)
{
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    multex = new Semaphore("multex", 1);

    numwait = 0;
    exitcode = 0;

    fileTable = new FILE*[MAX_FILE];
    filemap = new Bitmap(MAX_FILE);
    filemap->Mark(0);
    filemap->Mark(1);

    thread = NULL;

    if (id == 0)
    {
        this->parentID = 0;
    }
    else
    {
        this->parentID = kernel->currentThread->processID;
    }
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
    if (thread != NULL)
    {
        thread->Finish();
        delete thread;
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
}

int PCB::Exec(char *filename, int id)
{
    int* idx = new int;
    *idx = id;
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
    this->thread->Fork((VoidFunctionPtr)StartProcess_2, (void*) id);

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

char* PCB::GetFileName()
{
    return thread->name;
}

void StartProcess_2(int id)
{
    cout << "StartProcess_2 " << id << endl;
    // Lay fileName cua process id nay
    char *fileName = kernel->pTab->GetFileName(id);

    AddrSpace *space;
    space = new AddrSpace(fileName);

    if (space == NULL)
    {
        printf("\nPCB::Exec: Can't create AddSpace.");
        return;
    }

    kernel->currentThread->space = space;

    space->InitRegisters(); // set the initial register values
    space->RestoreState();  // load page table register

    kernel->machine->Run(); // jump to the user progam
    ASSERT(FALSE);          // machine->Run never returns;
                            // the address space exits
                            // by doing the syscall "exit"
}