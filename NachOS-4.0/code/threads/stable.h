#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"
#include "sem.h"
#define MAX_LOCK 10
#define MAX_SEMAPHORE 10

class STable
{
private:
    Bitmap *bm;
    Sem *semTab[MAX_SEMAPHORE];

public:
    STable();
    ~STable();
    int Create(char *name);
    int Wait(char *name);
    int Signal(char *name);
    int FindFreeSlot(int id);
};
#endif