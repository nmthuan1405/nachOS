#include "FileTable.h"
#include "synch.h"

class PCB
{
private:
    Semaphore* joinsem; // semaphore for join process
    Semaphore* exitsem; // semaphore for exit process
    Semaphore* multex;  // semaphore for exclusive access
    int exitcode;
    int numwait;        // number of process joined
    FileTable* fileTable;
public:
    int parentID;
public:
    PCB();
    PCB(int id);
    ~PCB();

    int Exec(char *filename, int pid);
    int GetID();
    int GetNumWait();
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    void SetExitCode(int ec);
    int GetExitCode();
    void SetFileName(char* fn);
    char* GetFileName();
};