#include "syscall.h"
#define OUTPUT_FILE "./test/output.txt"

typedef OpenFileId;

void getWater()
{
    int i;
    for (i = 0; i < 1000; i++)
    {
        // get water...
    }

    writeToFile(OUTPUT_FILE, GetCurrentProcessId());
}

void writeToFile(char *fileName, int id)
{
    OpenFileId f;

    f = Open(fileName, 0);
    if (f == -1)
    {
        PrintString("Error opening file\n");
        Exit(1);
    }

    id = id + '0';
    Write(&id, 1, f);
    Close(f);
}

int main()
{
    int i;
    OpenFileId outputId;
    char id;
    for (i = 0; i < 10; i++)
    {
        // Acquire voi nuoc
        Wait("water");

        // Lay 1 lit nuoc
        getWater();

        // Release voi nuoc
        Signal("water");
    }

    return 0;
}