#ifndef STABLE_H
#define STABLE_H
#define MAX_SEMAPHORE 10
#include "string.h"
#include "bitmap.h"
#include "sem.h"

class Sem;
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
    int FindFreeSlot();
};
#endif