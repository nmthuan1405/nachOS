#include "PCB.h"

PCB::PCB(int id)
{
    this->joinsem = new Semaphore("joinsem", 0);
    this->exitsem = new Semaphore("exitsem", 0);
    this->multex = new Semaphore("multex", 1);

    this->numwait = 0;
    this->exitcode = 0;
    this->thread = NULL;

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
    if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(thread != NULL)
	{		
		thread->Finish();
        delete thread;
	}
}

void PCB::JoinWait()
{
    joinsem->P();
}

void PCB::JoinRelease()
{
    joinsem->V();
}

void PCB::ExitWait()
{
    exitsem->P();
}

void PCB::ExitRelease()
{
    exitsem->V();
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
    this->thread->Fork((VoidFunctionPtr)StartProcess_2, (void *)id);

    multex->V();
    // Trả về id.
    return id;
}

void StartProcess_2(int id)
{
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